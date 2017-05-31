// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include "glm/glm.hpp"

// specify that we want the OpenGL core profile before including GLFW headers
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define NUMBER_OF_IMAGES 6

using namespace std;
using namespace glm;
using std::vector;
using std::string;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
void GreyScale(int luminanceFunction);


// globals
float scaleX = 0;
float scaleY = 0;

float windowWidth = 512.f;
float windowHeight = 512.f;

float textureWidth = 512.f;
float textureHeight = 512.f;


int command = 1;
int image = 1;
int hue = 0;
float exposureAdjustment = 0;
int gaussianSize = 1;
int sobel = 0;

float currentX = 0;
float currentY = 0;

float centerX = 0;
float centerY = 0;

float temp;

int click = 0;

int colorConversion = 0;
// three vertex positions and assocated colors of a triangle

int rightTurn = 0;
int leftTurn = 0;
int goDown = 0;
int goUp = 0;
float theta = 0;
float panningTheta = theta;
float prevTheta = 0;

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

struct MyShader
{
	// OpenGL names for vertex and fragment shaders, shader program
	GLuint  vertex;
	GLuint  fragment;
	GLuint  program;

	// initialize shader and program names to zero (OpenGL reserved value)
	MyShader() : vertex(0), fragment(0), program(0)
	{}
};
MyShader shader;
// load, compile, and link shaders, returning true if successful
bool InitializeShaders(MyShader *shader)
{
	// load shader source from files
	string vertexSource = LoadSource("vertex.glsl");
	string fragmentSource = LoadSource("fragment.glsl");
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	shader->vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	shader->fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	shader->program = LinkProgram(shader->vertex, shader->fragment);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate shader-related objects
void DestroyShaders(MyShader *shader)
{
	// unbind any shader programs and destroy shader objects
	glUseProgram(0);
	glDeleteProgram(shader->program);
	glDeleteShader(shader->vertex);
	glDeleteShader(shader->fragment);
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing textures

struct MyTexture
{
	GLuint textureID;
	GLuint target;
	int width;
	int height;

	// initialize object names to zero (OpenGL reserved value)
	MyTexture() : textureID(0), target(0), width(0), height(0)
	{}
};
MyTexture texture;

bool InitializeTexture(MyTexture* texture, const char* filename, GLuint target = GL_TEXTURE_2D)
{
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &texture->width, &texture->height, &numComponents, 0);
	if (data != nullptr)
	{
		texture->target = target;
		glGenTextures(1, &texture->textureID);
		glBindTexture(texture->target, texture->textureID);
		GLuint format = numComponents == 3 ? GL_RGB : GL_RGBA;
		//cout << numComponents << endl;
		glTexImage2D(texture->target, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

		// Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
		// GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Clean up
		glBindTexture(texture->target, 0);
		stbi_image_free(data);
		return !CheckGLErrors();
	}
	return true; //error
}

// deallocate texture-related objects
void DestroyTexture(MyTexture *texture)
{
	glBindTexture(texture->target, 0);
	glDeleteTextures(1, &texture->textureID);
}

void SaveImage(const char* filename, int width, int height, unsigned char *data, int numComponents = 3, int stride = 0)
{
	if (!stbi_write_png(filename, width, height, numComponents, data, stride))
		cout << "Unable to save image: " << filename << endl;
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing geometry data

struct MyGeometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colorBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;

	// initialize object names to zero (OpenGL reserved value)
	MyGeometry() : vertexBuffer(0), colorBuffer(0), vertexArray(0), elementCount(0)
	{}
};

MyGeometry geometry;
GLuint renderMode;

// create buffers and fill with geometry data, returning true if successful
bool InitializeGeometry(MyGeometry *geometry)
{
	float verticalScale = 1.f;
	float horizontalScale = 1.f;
	if (textureHeight < textureWidth){
		verticalScale = textureHeight/textureWidth;
	}
	else if (textureHeight > textureWidth){
		horizontalScale = textureWidth/textureHeight;
	}

	const GLfloat vertices[6][2] = {
		{ -1.f * horizontalScale, -1.f * verticalScale},
		{ -1.f * horizontalScale,  1.f * verticalScale },
		{ 1.f * horizontalScale, 1.f * verticalScale },
		
		{ 1.f * horizontalScale, 1.f * verticalScale },
		{ 1.f * horizontalScale,  -1.f * verticalScale },
		{ -1.f * horizontalScale, -1.f * verticalScale }
	};

	const GLfloat textureCoords[][2] = {
		{0.f, 0.f},
		{0.f, textureHeight},
		{textureWidth, textureHeight},

		{textureWidth, textureHeight},
		{textureWidth, 0.f},
		{0.f, 0.f}
	};

	const GLfloat colors[][3] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f }
	};

	geometry->elementCount = 6;

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOR_INDEX = 1;
	const GLuint TEXTURE_INDEX = 2;

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//
	glGenBuffers(1, &geometry->textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

	// create another one for storing our colors
	glGenBuffers(1, &geometry->colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	// Tell openGL how the data is formatted
	glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
	glVertexAttribPointer(TEXTURE_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXTURE_INDEX);

	// assocaite the color array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
	glVertexAttribPointer(COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOR_INDEX);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(MyGeometry *geometry)
{
	// unbind and destroy our vertex array object and associated buffers
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &geometry->vertexArray);
	glDeleteBuffers(1, &geometry->vertexBuffer);
	glDeleteBuffers(1, &geometry->colorBuffer);
}

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void RenderScene(MyGeometry *geometry, MyTexture* texture, MyShader *shader)
{
	// clear screen to a dark grey color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader->program);
	glBindVertexArray(geometry->vertexArray);
	glBindTexture(texture->target, texture->textureID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
	glBindTexture(texture->target, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}


// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		command = 1;
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		command = 2;
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		command = 3;
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		command = 4;
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		command = 5;
	else if (key == GLFW_KEY_LEFT){
		if (command == 1){
			if ((colorConversion > 0) && (action = GLFW_PRESS)){
				colorConversion--;
			}
		}
		else if (command == 2){
			if((leftTurn == 1)){
				theta = (theta + (3.14159f/180.f));
				panningTheta = (panningTheta + (3.14159f/180.f));
			}
			if (action == GLFW_PRESS){
				leftTurn = 1;
			}
			else if (action == GLFW_RELEASE){
				leftTurn = 0;
			}
		}
		else if (command == 3 && action == GLFW_PRESS){
			if ((image > 0) && (action == GLFW_PRESS)){
				image--;
			}
			DestroyTexture(&texture);
			DestroyGeometry(&geometry);
			//textureFiles {"test.png", "image1-mandrill.png", "image2-uclogo.png", "image3-aerial.jpg", "image4-thirsk.jpg", "image5-pattern.png"};
			if (action == GLFW_PRESS){
				if (image == 0){
					InitializeTexture(&texture, "test.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 512.f;
					textureHeight = 512.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 1){
					InitializeTexture(&texture, "image1-mandrill.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 512.f;
					textureHeight = 512.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 2){
					InitializeTexture(&texture, "image2-uclogo.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 692.f;
					textureHeight = 516.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 3){
					InitializeTexture(&texture, "image3-aerial.jpg", GL_TEXTURE_RECTANGLE);
					textureWidth = 2000.f;
					textureHeight = 931.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 4){
					InitializeTexture(&texture, "image4-thirsk.jpg", GL_TEXTURE_RECTANGLE);
					textureWidth = 400.f;
					textureHeight = 591.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 5){
					InitializeTexture(&texture, "image5-pattern.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 2048.f;
					textureHeight = 1536.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 6){
					InitializeTexture(&texture, "1b3.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 500.f;
					textureHeight = 453.f;
					InitializeGeometry(&geometry);
				}
			}
		}

	}
	else if (key == GLFW_KEY_RIGHT){
		if (command == 1){
			if ((colorConversion < 6) && (action == GLFW_PRESS)){
				colorConversion++;
			}
		}
		else if (command == 2){
			if ((rightTurn == 1)){
				theta = (theta - (3.14159f/180.f));
				panningTheta = (panningTheta - (3.14159f/180.f));
				
			}
			if (action == GLFW_PRESS){
				rightTurn = 1;
			}
			else if (action == GLFW_RELEASE){
				rightTurn = 0;
			}
		}
		else if (command == 3 && action == GLFW_PRESS){
			if ((image < 6) && (action == GLFW_PRESS)){
				image++;
			}
			DestroyTexture(&texture);
			//textureFiles {"test.png", "image1-mandrill.png", "image2-uclogo.png", "image3-aerial.jpg", "image4-thirsk.jpg", "image5-pattern.png"};

			if (action == GLFW_PRESS){
				if (image == 0){
					InitializeTexture(&texture, "test.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 512.f;
					textureHeight = 512.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 1){
					InitializeTexture(&texture, "image1-mandrill.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 512.f;
					textureHeight = 512.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 2){
					InitializeTexture(&texture, "image2-uclogo.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 692.f;
					textureHeight = 516.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 3){
					InitializeTexture(&texture, "image3-aerial.jpg", GL_TEXTURE_RECTANGLE);
					textureWidth = 2000.f;
					textureHeight = 931.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 4){
					InitializeTexture(&texture, "image4-thirsk.jpg", GL_TEXTURE_RECTANGLE);
					textureWidth = 400.f;
					textureHeight = 591.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 5){
					InitializeTexture(&texture, "image5-pattern.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 2048.f;
					textureHeight = 1536.f;
					InitializeGeometry(&geometry);
				}
				else if (image == 6){
					InitializeTexture(&texture, "1b3.png", GL_TEXTURE_RECTANGLE);
					textureWidth = 500.f;
					textureHeight = 453.f;
					InitializeGeometry(&geometry);
				}
			}
		}
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		if ((command == 1) && (colorConversion == 5)){
			hue = (hue + 10)%360;
		}
		else if ((command == 1) && (colorConversion == 6)){
			exposureAdjustment += 0.1;
		}
		else if (command == 4){
			if (gaussianSize <= 5){
				gaussianSize += 2;
			}
		}
		else if (command == 5){
			if (sobel <= 2){
				sobel++;
			}
		}

	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		if ((command == 1) && (colorConversion == 5)){
			if ((hue - 10) >= 0)
				hue -= 10;
		}
		else if ((command == 1) && (colorConversion == 6)){
			exposureAdjustment -= 0.1;
		}
		else if (command == 4){
			if (gaussianSize > 1){
				gaussianSize -= 2;
			}	
		}
		else if (command == 5){
			if (sobel >= 1){
				sobel--;
			}
		}
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		click = 1;
	}	
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		click = 0;
	}
}

float prevX = 0.f;
float prevY = 0.f;

static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos){
	if (click == 1){
		centerX += 2 * ((xpos - prevX)/pow(2.71828, scaleX))/windowWidth;
		centerY += 2 * (-(ypos - prevY)/pow(2.71828, scaleY))/windowHeight;
	}
	prevX = xpos;
	prevY = ypos;
}

void ScrollCallback(GLFWwindow* window, double xOff, double yOff){	
	scaleX += (yOff/20);
	scaleY += (yOff/20);
}


// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(windowWidth, windowHeight, "CPSC 453 Assignment Two", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetCursorPosCallback(window, CursorPosCallBack);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwMakeContextCurrent(window);

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	if (!InitializeShaders(&shader)) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}

	// call function to create and fill buffers with geometry data
	if (!InitializeGeometry(&geometry))
		cout << "Program failed to intialize geometry!" << endl;

	if(!InitializeTexture(&texture, "image1-mandrill.png", GL_TEXTURE_RECTANGLE))
		cout << "Program failed to intialize texture!" << endl;


	glUseProgram(shader.program);
	GLint colorUniform = glGetUniformLocation(shader.program, "colorConversion");
	glUseProgram(shader.program);
	GLint hueUniform = glGetUniformLocation(shader.program, "hue");
	glUseProgram(shader.program);
	GLint exposureUniform = glGetUniformLocation(shader.program, "exposureAdjustment");
	
	// find scale uniform location
	glUseProgram(shader.program);
	GLint scaleUniform = glGetUniformLocation(shader.program, "scale");

	glUseProgram(shader.program);
	GLint gaussianUniform = glGetUniformLocation(shader.program, "gaussian");

	glUseProgram(shader.program);
	GLint sobelUniform = glGetUniformLocation(shader.program, "sobel");


	glUseProgram(shader.program);
	GLint dimensionUniform = glGetUniformLocation(shader.program, "dimension");

	glUseProgram(shader.program);
	GLint offsetUniform = glGetUniformLocation(shader.program, "offset");


	// run an event-triggered main loop
	while (!glfwWindowShouldClose(window))
	{
		
		glUseProgram(shader.program);
		glUniform1f(colorUniform, colorConversion);
		glUseProgram(shader.program);
		glUniform1f(hueUniform, hue);
		glUseProgram(shader.program);
		glUniform1f(exposureUniform, exposureAdjustment);

		// create scale matrix, use the shader, update vertex uniform
		float scaleMatrix[4] {
			(cos(theta) * pow(2.71828, scaleX)), (-sin(theta) * pow(2.71828, scaleY)),
			(sin(theta) * pow(2.71828, scaleX)), (cos(theta) * pow(2.71828, scaleY))
		};
		glUseProgram(shader.program);
		glUniformMatrix2fv(scaleUniform, 1, GL_TRUE, scaleMatrix);

		panningTheta = theta - prevTheta;

		float temp2 = prevTheta;
		temp = centerX;
		centerX = temp * cos(-panningTheta) + centerY * sin(-panningTheta);
		centerY = centerY * cos(-panningTheta) - temp * sin(-panningTheta);

		prevTheta = (panningTheta == 0) ? temp2 : panningTheta + temp2;


		glUseProgram(shader.program);
		glUniform2f(offsetUniform, centerX * pow(2.71828, scaleX), centerY * pow(2.71828, scaleX));


		glUseProgram(shader.program);
		glUniform1i(gaussianUniform, gaussianSize);

		glUseProgram(shader.program);
		glUniform1i(sobelUniform, sobel);

		// call function to draw our scene
		// call function to draw our scene
		RenderScene(&geometry, &texture, &shader); //render scene with texture

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exit
	DestroyGeometry(&geometry);
	DestroyShaders(&shader);
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}

void GreyScale(int luminanceFunction){

}