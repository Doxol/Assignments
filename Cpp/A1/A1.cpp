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

//Name: David Sepulveda

//CPSC 453 Assignment #1 Programming Component

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>
#include <math.h>
#include "./glm/glm.hpp"



// specify that we want the OpenGL core profile before including GLFW headers
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

using namespace std;

using std::vector;
using glm::vec2;
using glm::vec3;
vector<vec2> points;
vector<vec3> colors;


// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
int currentImage = 0;
int level = 1;
float revolutions = 1;
int degrees = 45;
int startAngle = 0;
int endAngle = 360;
int triangulars = 90;
int sierpinskiNest = 0;
int barnsleyDots = 100000;
int depth = 2;


void createCircle(float radius, vector<float>& vertices, vector<float>& colors);
void createSpiral(float radius, int angle, int endAngle, float revolutions, vector<float>& vertices, vector<float>& colors);
void createSquare(float sideLength, float angle, vector<float>& vertices, vector<float>& colors, float red, float green, float blue);
void nestedSquares(float sideLength, float angle, int level, vector<float>& vertices, vector<float>& colors);
void createTriangle(float sideLength, float angle, float xStart, float yStart, vector<float>& vertices, vector<float>& colors, float red, float green, float blue, int nest);
void sierpinskiControl(float sideLength, float angle, float xStart, float yStart, vector<float>& vertices, vector<float>& colors, float red, float green, float blue, int nest);
void sierpinskiTriangle(float sideLength, float angle, float xStart, float yStart, vector<float>& vertices, vector<float>& colors, float red, float green, float blue, int nest);
void ninjaStarOne(float sideLength, float angle, vector<float>& vertices, vector<float>& colors, float red, float green, float blue);
void ninjaStarTwo(float sideLength, float angle, vector<float>& vertices, vector<float>& colors, float red, float green, float blue);
void barnsleyFern(int seed, int dots, vector<float>& vertices, vector<float>& colors);





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
// Functions to set up OpenGL buffers for storing geometry data

struct MyGeometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  colorBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;

	// initialize object names to zero (OpenGL reserved value)
	MyGeometry() : vertexBuffer(0), colorBuffer(0), vertexArray(0), elementCount(0)
	{}
};
	//Globals
	MyGeometry geometry;
	GLuint renderMode;

// create buffers and fill with geometry data, returning true if successful
bool InitializeGeometry(MyGeometry *geometry, const vector<float>& vertices, const vector<float>& colors)
{
	//divide by two because there is an xcomponent and ycomponent for each vertex
	geometry->elementCount = vertices.size() / 2;

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint color_INDEX = 1;

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// create another one for storing our colors
	glGenBuffers(1, &geometry->colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colors.size(), &colors[0], GL_STATIC_DRAW);

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	// assocaite the color array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
	glVertexAttribPointer(color_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(color_INDEX);

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

void RenderScene(MyGeometry *geometry, MyShader *shader, GLuint renderMode = GL_TRIANGLES)
{
	// clear screen to a dark grey color
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader->program);
	glBindVertexArray(geometry->vertexArray);
	glDrawArrays(renderMode, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if(key == GLFW_KEY_1 && action == GLFW_PRESS){
		currentImage = 1;
		level = 1;
		degrees = 45;
		revolutions = 1;
		sierpinskiNest = 0;
		endAngle = 360;
		startAngle = 0;
		barnsleyDots = 100000;
		triangulars = 90;
		depth = 2;
		DestroyGeometry(&geometry);
		vector<float> vertices;
		vector<float> colors;
		createCircle(.5f, vertices, colors);
		InitializeGeometry(&geometry, vertices, colors);
		renderMode = GL_LINE_STRIP;     
	}
	else if(key == GLFW_KEY_2 && action == GLFW_PRESS){
		currentImage = 2;
		level = 1;
		degrees = 45;
		revolutions = 1;
		startAngle = 0;
		sierpinskiNest = 0;
		triangulars = 90;
		barnsleyDots = 100000;
		endAngle = 360;
		depth = 2;
		DestroyGeometry(&geometry);
		vector<float> vertices;
		vector<float> colors;
		createSpiral(.0005f, startAngle, endAngle, revolutions, vertices, colors);
		InitializeGeometry(&geometry, vertices, colors);
		renderMode = GL_LINE_STRIP;     
	}
	else if(key == GLFW_KEY_3 && action == GLFW_PRESS){
		currentImage = 3;
		level = 1;
		revolutions = 1;
		triangulars = 90;
		degrees = 45;
		sierpinskiNest = 0;
		endAngle = 360;
		barnsleyDots = 100000;
		startAngle = 0;
		depth = 2;
		DestroyGeometry(&geometry);
		vector<float> vertices;
		vector<float> colors;
		createSquare(0.75f, 0.f, vertices, colors, 1.f, 0.f, 0.f);
		InitializeGeometry(&geometry, vertices, colors);
		renderMode = GL_LINE_STRIP;     
	}
	else if(key == GLFW_KEY_4 && action == GLFW_PRESS){
		currentImage = 4;
		level = 1;
		revolutions = 1;
		triangulars = 90;
		sierpinskiNest = 0;
		degrees = 45;
		endAngle = 360;
		barnsleyDots = 100000;
		startAngle = 0;
		depth = 2;
		DestroyGeometry(&geometry);
		vector<float> vertices;
		vector<float> colors;
		nestedSquares(1.5f, degrees, level, vertices, colors);
		InitializeGeometry(&geometry, vertices, colors);
		renderMode = GL_LINES;
	}
	else if(key == GLFW_KEY_5 && action == GLFW_PRESS){
		currentImage = 5;
		level = 1;
		revolutions = 1;
		degrees = 45;
		sierpinskiNest = 0;
		triangulars = 90;
		endAngle = 360;
		barnsleyDots = 100000;
		startAngle = 0;
		depth = 2;
		DestroyGeometry(&geometry);
		vector<float> vertices;
		vector<float> colors;
		sierpinskiControl(1.f, 90.f, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 0);
		createTriangle(1.f, 90.f, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 0);
		InitializeGeometry(&geometry, vertices, colors);
		renderMode = GL_TRIANGLES;
	}
	else if(key == GLFW_KEY_6 && action == GLFW_PRESS){
		currentImage = 6;
		level = 1;
		revolutions = 1;
		degrees = 45;
		sierpinskiNest = 0;
		triangulars = 90;
		endAngle = 360;
		startAngle = 0;
		barnsleyDots = 100000;
		depth = 2;
		DestroyGeometry(&geometry);
		vector<float> vertices;
		vector<float> colors;
		barnsleyFern(1, barnsleyDots, vertices, colors);
		InitializeGeometry(&geometry, vertices, colors);
		renderMode = GL_POINTS;
	}
	else if(key == GLFW_KEY_UP && action == GLFW_PRESS){
		if (currentImage == 2){
			revolutions++;
			currentImage = 2;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			createSpiral(.0005f, startAngle, endAngle, revolutions, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINE_STRIP;
		}
		else if (currentImage == 4){
			level++;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			nestedSquares(1.5f, degrees, level, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINES;
		}
		else if (currentImage == 5){
			if (sierpinskiNest <= 6){
				sierpinskiNest++;
			}
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			sierpinskiControl(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 1);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_TRIANGLES;
		}
		else if (currentImage == 6){
			if (barnsleyDots <= 1000000000){
				barnsleyDots += 3000;
			}
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			barnsleyFern(1, barnsleyDots, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_POINTS;
		}
	}
	else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		if (currentImage == 2){
			if (revolutions > 1){
				revolutions--;
			}
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			createSpiral(.0005f, startAngle, endAngle, revolutions, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINE_STRIP;
		}
		else if (currentImage == 4){
			if (level > 1){
				level--;
			}
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			nestedSquares(1.5f, degrees, level, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINES;
		}
		else if (currentImage == 5){
			if (sierpinskiNest > 0){
				sierpinskiNest--;
			}
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			if (sierpinskiNest == 0){
				createTriangle(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 0);
			}
			else{
				sierpinskiControl(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 1);
			}		
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_TRIANGLES;
		}
		else if (currentImage == 6){
			if (barnsleyDots >= 6000){
				barnsleyDots -= 3000;
			}
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			barnsleyFern(1, barnsleyDots, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_POINTS;
		}
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		if (currentImage == 4){
			degrees = (degrees + 5)%360;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			nestedSquares(1.5f, degrees, level, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINES;
		}
		else if (currentImage == 2){
			startAngle = (startAngle + 5)%360;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			endAngle = (startAngle + 360);
			createSpiral(.0005f, startAngle, endAngle, revolutions, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINE_STRIP;
		}
		else if (currentImage == 5){
			triangulars = (triangulars + 5)%360;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			if (sierpinskiNest == 0){
				createTriangle(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 0);
			}
			else{
				sierpinskiControl(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 1);
			}	
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_TRIANGLES;
		}

	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
		if (currentImage == 4){
			degrees = (degrees - 5)%360;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			nestedSquares(1.5f, degrees, level, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINES;
		}
		else if (currentImage == 2){
			startAngle = (startAngle - 5)%360;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			endAngle = (startAngle + 360);
			createSpiral(.0005f, startAngle, endAngle, revolutions, vertices, colors);
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_LINE_STRIP;
		}
		else if (currentImage == 5){
			triangulars = (triangulars - 5)%360;
			DestroyGeometry(&geometry);
			vector<float> vertices;
			vector<float> colors;
			if (sierpinskiNest == 0){
				createTriangle(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 0);
			}
			else{
				sierpinskiControl(1.f, triangulars, 0, 0, vertices, colors, 1.f, 0.f, 0.f, 1);
			}	
			InitializeGeometry(&geometry, vertices, colors);
			renderMode = GL_TRIANGLES;
		}

	}
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{   
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(600, 600, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwMakeContextCurrent(window);

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	MyShader shader;
	if (!InitializeShaders(&shader)) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}

	renderMode = GL_TRIANGLES;
	//-- create a triangle
	vector<float> vertices(6);
	vertices[0] = -.75f;
	vertices[1] = -.75f;
	
	vertices[2] = 0.f;
	vertices[3] = .75f;

	vertices[4] = .75f;
	vertices[5] = -.75f;

	vector<float> colors(9);
	colors[0] = 1;
	colors[1] = 0;
	colors[2] = 0;

	colors[3] = 0;
	colors[4] = 1;
	colors[5] = 0;

	colors[6] = 0;
	colors[7] = 0;
	colors[8] = 1;


	// call function to create and fill buffers with geometry data

	if (!InitializeGeometry(&geometry, vertices, colors))
		cout << "Program failed to intialize geometry!" << endl;

	// run an event-triggered main loop
	while (!glfwWindowShouldClose(window))
	{
		// call function to draw our scene
		RenderScene(&geometry, &shader, renderMode);

		// scene is rendered to the back buffer, so swap to front for display
		glfwSwapBuffers(window);

		// sleep until next event before drawing again
		glfwWaitEvents();
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
	string version  = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver  = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
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
string LoadSource(const string &filename){
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
GLuint CompileShader(GLenum shaderType, const string &source){
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
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader){
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

void createCircle(float radius, vector<float>& vertices, vector<float>& colors){
	float red = 1, green = 0, blue = 0;
	for(int angle = 0; angle <= 360; ++angle)
	{
		float rad = (float)angle * 3.14159265f/180.f;
		vertices.push_back(radius * cos(rad));
		vertices.push_back(radius * sin(rad));

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		red -= 1.f/360.f;
		green += 1.f/360.f;
		blue += .5f/360.f;
	}

}

void createSpiral(float radius, int angle, int endAngle, float revolutions, vector<float>& vertices, vector<float>& colors){
	float red = 1, green = 0, blue = 0;
	float rad;

	radius = 1/((endAngle - startAngle) * revolutions);

	for(int start = angle; start <= endAngle * revolutions; start++){
		rad = (float)start * 3.14159265f/180.f;
		vertices.push_back((radius * start) * cos(rad));
		vertices.push_back((radius * start) * sin(rad));

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		blue += 1/((endAngle * revolutions) - start);		
	}
}

/*
 * Draws a point at a corner, turns 90 degrees, draws next corner.
 */
void createSquare(float sideLength, float angle, vector<float>& vertices, vector<float>& colors, float red, float green, float blue){
	float rad = (float)angle * 3.14159265f/180.f;

	for (int i = 0; i < 4; i++){
		vertices.push_back(cos(rad) * sideLength);
		vertices.push_back(sin(rad) * sideLength);

		angle += 90;
		rad = (float)angle * 3.14159265f/180.f;

		vertices.push_back(cos(rad) * sideLength);
		vertices.push_back(sin(rad) * sideLength);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);
	}

}

/*
 * Draws the squares, rotates them and changes the side lengths and then draws the diamonds.
 */
void nestedSquares(float sideLength, float angle, int level, vector<float>& vertices, vector<float>& colors){
	float red = 0.1, green = 0.5, blue = 1;
	float redD = 1, greenD = 1, blueD = 1;

	for (int i = 0; i < level; i++){
		createSquare(sideLength/2, angle + 45, vertices, colors, red, green, blue);
		createSquare((sideLength)/sqrt(2), angle, vertices, colors, redD, greenD, blueD);

		sideLength /= 2;
		red += 0.08;
		green += 0.08;
		blue -= 0.08;

		redD -= 0.25;
		greenD -= 0.25;
		blueD -= 0.25;

	}
}

void createTriangle(float sideLength, float angle, float xStart, float yStart, vector<float>& vertices, vector<float>& colors, float red, float green, float blue, int nest){
	float rad = float(angle) * 3.14159265f/180.f;

	for (int i = 0; i < 3; i++){
		vertices.push_back(xStart + cos(rad) * sideLength);
		vertices.push_back(yStart + sin(rad) * sideLength);

		angle += 120;
		rad = float(angle) * 3.14159265f/180.f;
	}

	colors.push_back(red);
	colors.push_back(blue);
	colors.push_back(green);
	
	colors.push_back(blue);
	colors.push_back(green);
	colors.push_back(red);
	
	colors.push_back(green);
	colors.push_back(red);
	colors.push_back(blue);
}

void sierpinskiControl(float sideLength, float angle, float xStart, float yStart, vector<float>& vertices, vector<float>& colors, float red, float green, float blue, int nest){

	sierpinskiTriangle(sideLength, angle, xStart, yStart, vertices, colors, red, green, blue, nest);
	if (nest < sierpinskiNest){
		nest++;
		sierpinskiControl(sideLength/2, angle, xStart, yStart + (sideLength/2), vertices, colors, red, green, blue + 0.25, nest);
		sierpinskiControl(sideLength/2, angle, xStart + (sqrt(3) * sideLength/4), yStart - (sideLength/4), vertices, colors, red + 0.25, green, blue, nest);
		sierpinskiControl(sideLength/2, angle, xStart - (sqrt(3) * sideLength/4), yStart - (sideLength/4), vertices, colors, red, green + 0.25, blue, nest);
	}
}

void sierpinskiTriangle(float sideLength, float angle, float xStart, float yStart, vector<float>& vertices, vector<float>& colors, float red, float green, float blue, int nest){
	float rad = float(angle) * 3.14159265f/180.f;

	for (int i = 0; i < 3; i++){
		vertices.push_back(xStart + cos(rad) * sideLength);
		vertices.push_back(yStart + sin(rad) * sideLength);

		angle += 120;
		rad = float(angle) * 3.14159265f/180.f;
	}

	angle += 60;
	rad = float(angle) * 3.14159265f/180.f;

	for (int i = 0; i < 3; i++){
		vertices.push_back(xStart + cos(rad) * (sideLength/2));
		vertices.push_back(yStart + sin(rad) * (sideLength/2));

		angle += 120;
		rad = float(angle) * 3.14159265f/180.f;
	}

	colors.push_back(red);
	colors.push_back(blue);
	colors.push_back(green);
	
	colors.push_back(blue);
	colors.push_back(green);
	colors.push_back(red);
	
	colors.push_back(green);
	colors.push_back(red);
	colors.push_back(blue);
	
	colors.push_back(0.2);
	colors.push_back(0.2);
	colors.push_back(0.2);

	colors.push_back(0.2);
	colors.push_back(0.2);
	colors.push_back(0.2);

	colors.push_back(0.2);
	colors.push_back(0.2);
	colors.push_back(0.2);
}

void ninjaStarOne(float sideLength, float angle, vector<float>& vertices, vector<float>& colors, float red, float green, float blue){
	float rad = (float)angle * 3.14159265f/180.f;

	for (int i = 0; i < 3; i++){
		vertices.push_back(cos(rad) * sideLength);
		vertices.push_back(sin(rad) * sideLength);

		vertices.push_back(cos(rad) * sideLength/2);
		vertices.push_back(sin(rad) * sideLength/2);

		angle += 120;
		rad = (float)angle * 3.14159265f/180.f;

		vertices.push_back(cos(rad) * sideLength);
		vertices.push_back(sin(rad) * sideLength);


		angle += 120;
		rad = (float)angle * 3.14159265f/180.f;

		vertices.push_back(cos(rad) * sideLength/2);
		vertices.push_back(sin(rad) * sideLength/2);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);
	}
}

void ninjaStarTwo(float sideLength, float angle, vector<float>& vertices, vector<float>& colors, float red, float green, float blue){
	float rad = (float)angle * 3.14159265f/180.f;

	for (int i = 0; i < 3; i++){
		vertices.push_back(cos(rad) * sideLength);
		vertices.push_back(sin(rad) * sideLength);

		angle += 120;
		rad = (float)angle * 3.14159265f/180.f;

		vertices.push_back(cos(rad) * (sideLength/2));
		vertices.push_back(sin(rad) * (sideLength/4));
		
		angle += 120;
		rad = (float)angle * 3.14159265f/180.f;

		vertices.push_back(cos(rad) * (sideLength/2));
		vertices.push_back(sin(rad) * (sideLength/2));

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);

		colors.push_back(red);
		colors.push_back(green);
		colors.push_back(blue);
	}
}

void barnsleyFern(int seed, int dots, vector<float>& vertices, vector<float>& colors){
	int red = 1, green = 1, blue = 1;
	int probability;
	float x = 0;
	float y = 0;
	float temp;
	for (int i = 0; i < dots; i++){
		
		vertices.push_back(x * .15f - 0.2);
		vertices.push_back(y * .15f - 0.6);

		colors.push_back(red - (0.16 * y));
		colors.push_back(green - (0.08 * y));
		colors.push_back(blue - (0.16 * y));


		probability = rand() % 100;
		if (probability == 99){
			x = 0;
			y = 0.16 * y;
		}
		else if((probability >= 92)){
			temp = x;
			x = (-0.15 * x) + (0.28 * y);
			y = (0.26 * temp) + (0.24 * y) + 0.44;
		}
		else if((probability >= 85)){
			temp = x;
			x = (0.2 * x) - (0.26 * y);
			y = (0.23 * temp) + (0.22 * y) + 1.6;
		}
		else{
			temp = x;
			x = (0.85 * x) + (0.04 * y);
			y = (-0.04 * temp) + (0.85 * y) + 1.6;
		}
	}
}

// ==========================================================================
