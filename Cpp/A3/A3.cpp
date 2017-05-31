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

// Specify that we want the OpenGL core profile before including GLFW headers
#ifndef LAB_LINUX
	#include <glad/glad.h>
#else
	#define GLFW_INCLUDE_GLCOREARB
	#define GL_GLEXT_PROTOTYPES
#endif
#include <GLFW/glfw3.h>

//STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <vector>
#include "GlyphExtractor.h"
using namespace std;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint TCSshader, GLuint TESshader, GLuint fragmentShader);

void drawTeacup();
void drawFish();
void bluePurple(vector<float>& colours);
void arrow(vector<float>& vertices);
void cross(vector<float>& vertices);
void pushPieces(float tempX, float tempY, vector<float>& vertices);
void pushLines(float tempX, float tempY, float tempXEnd, float tempYEnd, vector<float>& vertices);
void solidColor(vector<float>& colours, float r, float g, float b, int repeat);
float drawGlyphs(char letter, GlyphExtractor extractor, vector<float>& vertices, vector<float>& colours, float advance, float r, float g, float b);
void drawString(string things);
void drawTheString();
void drawStrings(vector<string> tadaa);
void drawDaDaa();
int findClosePoint();


int controlPoints = 3;

float tempX1;
float tempX2;
float tempX3;
float tempX4;

float tempY1;
float tempY2;
float tempY3;
float tempY4;

float startOff = 0.f;

float line = 0.f;
float lineSize = 0.8f;

vector<float> vertices;
vector<float> colours;


float windowWidth = 512.f;
float windowHeight = 512.f;


int caps = 0;

int shift = 0;

string font = "lora/Lora-Regular.ttf";

string lastWorkingFont = "lora/Lora-Regular.ttf";

string myThingy;

string spookyWords = "Daaaaaaa da dadadadaaaaaaa da dadadadaaaaaaaa da dadadadaaaaaaaaaaa";

float startX = 0.f;
float startY = -0.63f;

float currentX;
float currentY;

float scale = 0.f;
float scaleY = 0.f;

float offset = 0.f;
float offsetY = 0.f;

float lastX = 0.f;

float firstX = 0.f;

float lastAdvance = 0.f;

int thingsLength = 0;

float totalAdvance = 0.f;

float previousThingsLength = 0.f;

float windowScale = 0.4f;

float previousScale = 0.4f;

int spookyEffect = 0;



const int TTF = 1;
const int OTF = 2;
// input other font types as needed
string fontType = "ttf";


// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

struct MyShader{
	// OpenGL names for vertex and fragment shaders, shader program
	GLuint  vertex;
	GLuint  TCS; 
	GLuint  TES; 
	GLuint  fragment;
	GLuint  program;

	// initialize shader and program names to zero (OpenGL reserved value)
	MyShader() : vertex(0), fragment(0), program(0)
	{}
};
MyShader shader;

// load, compile, and link shaders, returning true if successful
bool InitializeShaders(MyShader *shader){
	// load shader source from files
	string vertexSource = LoadSource("vertex.glsl");
	string fragmentSource = LoadSource("fragment.glsl");
	string TCSSource = LoadSource("tessControl.glsl");
	string TESSource = LoadSource("tessEval.glsl"); 
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	shader->vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	shader->fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource); 
	shader->TCS = CompileShader(GL_TESS_CONTROL_SHADER, TCSSource);
	shader->TES = CompileShader(GL_TESS_EVALUATION_SHADER, TESSource);

	// link shader program
	shader->program = LinkProgram(shader->vertex, shader->TCS, shader->TES, shader->fragment);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate shader-related objects
void DestroyShaders(MyShader *shader){
	// unbind any shader programs and destroy shader objects
	glUseProgram(0);
	glDeleteProgram(shader->program);
	glDeleteShader(shader->vertex);
	glDeleteShader(shader->fragment);
	glDeleteShader(shader->TCS); 
	glDeleteShader(shader->TES); 
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing textures

struct MyTexture{
	GLuint textureID;
	GLuint target;
	int width;
	int height;

	// initialize object names to zero (OpenGL reserved value)
	MyTexture() : textureID(0), target(0), width(0), height(0)
	{}
};

bool InitializeTexture(MyTexture* texture, const char* filename, GLuint target = GL_TEXTURE_2D){
	int numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(filename, &texture->width, &texture->height, &numComponents, 0);
	if (data != nullptr)
	{
		texture->target = target;
		glGenTextures(1, &texture->textureID);
		glBindTexture(texture->target, texture->textureID);
		GLuint format = numComponents == 3 ? GL_RGB : GL_RGBA;
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
void DestroyTexture(MyTexture *texture){
	glBindTexture(texture->target, 0);
	glDeleteTextures(1, &texture->textureID);
}

void SaveImage(const char* filename, int width, int height, unsigned char *data, int numComponents = 3, int stride = 0){
	if (!stbi_write_png(filename, width, height, numComponents, data, stride))
		cout << "Unable to save image: " << filename << endl;
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing geometry data

struct MyGeometry{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  textureBuffer;
	GLuint  colourBuffer;
	GLuint  vertexArray;
	GLsizei elementCount;

	// initialize object names to zero (OpenGL reserved value)
	MyGeometry() : vertexBuffer(0), colourBuffer(0), vertexArray(0), elementCount(0)
	{}
};

MyGeometry geometry;


// create buffers and fill with geometry data, returning true if successful
bool InitializeGeometry(MyGeometry *geometry, const vector<float>& vertices, const vector<float>& colours){

	//divide by two because there is an xcomponent and ycomponent for each vertex
	geometry->elementCount = vertices.size() / 2;

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint VERTEX_INDEX = 0;
	const GLuint COLOUR_INDEX = 1;

	// create an array buffer object for storing our vertices
	glGenBuffers(1, &geometry->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// create another one for storing our colours
	glGenBuffers(1, &geometry->colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colours.size(), &colours[0], GL_STATIC_DRAW);

	// create a vertex array object encapsulating all our vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// associate the position array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
	glVertexAttribPointer(VERTEX_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_INDEX);

	// assocaite the colour array with the vertex array object
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
	glVertexAttribPointer(COLOUR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOUR_INDEX);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(MyGeometry *geometry){
	// unbind and destroy our vertex array object and associated buffers
	vertices.clear();
	colours.clear();

	vector<float>().swap(vertices);
	vector<float>().swap(colours);

	string tempString;
	tempString = myThingy;
	string().swap(myThingy);
	myThingy = tempString;

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &geometry->vertexArray);
	glDeleteBuffers(1, &geometry->vertexBuffer);
	glDeleteBuffers(1, &geometry->colourBuffer);
}

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void RenderScene(MyGeometry *geometry, MyShader *shader){
	// clear screen to a dark grey colour
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader->program);
	glBindVertexArray(geometry->vertexArray);
	glDrawArrays(GL_PATCHES, 0, geometry->elementCount);
	//glDrawArrays(GL_LINE_STRIP, 0, geometry->elementCount);

	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description){
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS){
		if (shift == 0){
			DestroyGeometry(&geometry);
			spookyEffect = 0;
			drawFish();
			offsetY = 0.f;
			scaleY = 0.f;
			scale = 0.f;
			offset = 0.f;
			totalAdvance /= windowScale;
			windowScale = 0.1f;
			totalAdvance *= windowScale;
		}
		else {
			DestroyGeometry(&geometry);
			spookyEffect = 0;
			myThingy += '!';
			drawTheString();
		}
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		spookyEffect = 0;
		drawTeacup();
		scale = 0.f;
		offset = 0.f;
		offsetY = 0.f;
		scaleY = 0.f;
		totalAdvance /= windowScale;
		windowScale = 0.4f;
		totalAdvance *= windowScale;
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		spookyEffect = 0;
		offsetY = 0.f;
		scaleY = 0.f;
		windowScale = 0.4f;
		myThingy = "David";
		drawTheString();
	}
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		spookyEffect = 0;
		myThingy = "The quick brown fox jumps over the lazy dog.";
		scale = -0.01f;
		offset = 0;
		offsetY = 0.f;
		scaleY = 0.f;
		windowScale = 0.2f;
		drawTheString();
	}
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		if (shift == 1){
			spookyEffect = 0;
			myThingy += "%";
		}
		else{
			myThingy = spookyWords;
			spookyEffect = 1;
			scale = 0.f;
			offset = 0.8f;
			offsetY = -1.f;
			scaleY = 0.01f;
		}
		drawTheString();
	}
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		if (shift == 1){
			spookyEffect = 0;
			myThingy += "^";
		}
		else{
			myThingy = spookyWords;
			spookyEffect = 2;
			scale = 0.f;
			offset = 0.8f;
			offsetY = -1.f;
			scaleY = 0.01f;
		}
		drawTheString();
	}
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		if (shift == 1){
			spookyEffect = 0;
			myThingy += "&";
		}
		else{
			myThingy = spookyWords;
			spookyEffect = 3;
			scale = 0.f;
			offset = 0.8f;
			offsetY = -1.f;
			scaleY = 0.01f;
		}
		drawTheString();
	}
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		if (shift == 1){
			spookyEffect = 0;
			myThingy += "&";
			drawTheString();
		}
		else{
			myThingy = spookyWords;
			spookyEffect = 4;
			scale = 0.f;
			offset = 0.f;
			offsetY = -1.f;
			scaleY = 0.005f;
			drawDaDaa();
		}
	}
	else if (key == GLFW_KEY_9 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		if (shift == 1){
			spookyEffect = 0;
			myThingy += "(";
			drawTheString();
		}
		else{
			spookyEffect = 0;
			font = "alex-brush/AlexBrush-Regular.ttf";
			drawTheString();
		}
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		if (shift == 1){
			spookyEffect = 0;
			myThingy += ")";
			drawTheString();
		}
		else{
			spookyEffect = 0;
			font = "source-sans-pro/SourceSansPro-Regular.otf";
			drawTheString();
		}
	}
	else if (((key == GLFW_KEY_LEFT_SHIFT) || (key == GLFW_KEY_RIGHT_SHIFT)) && action == GLFW_PRESS){
		shift = 1;
	}
	else if (((key == GLFW_KEY_LEFT_SHIFT) || (key == GLFW_KEY_RIGHT_SHIFT)) && action == GLFW_RELEASE){
		shift = 0;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
		if (!(spookyEffect >= 1))
			scale -= 0.001f;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
		if (!(spookyEffect >= 1))
			scale += 0.001f;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS){
		if (spookyEffect >= 1){
			scaleY += 0.001f;
		}
		else {
			totalAdvance /= windowScale;
			windowScale += 0.05f;
			totalAdvance *= windowScale;
		}
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		if (spookyEffect >= 1){
			scaleY -= 0.001f;
		}
		else {
			totalAdvance /= windowScale;
			windowScale -= (windowScale >= 0.1f) ? 0.05f : 0.f;
			totalAdvance *= windowScale;
		}
	}
	else if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS){
		caps = (caps == 0) ? 1 : 0;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'a' : 'A';
		drawTheString();
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'b' : 'B';
		drawTheString();
	}
	else if (key == GLFW_KEY_C && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'c' : 'C';
		drawTheString();
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'd' : 'D';
		drawTheString();
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'e' : 'E';
		drawTheString();
	}
	else if (key == GLFW_KEY_F && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'f' : 'F';
		drawTheString();
	}
	else if (key == GLFW_KEY_G && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'g' : 'G';
		drawTheString();
	}
	else if (key == GLFW_KEY_H && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'h' : 'H';
		drawTheString();
	}
	else if (key == GLFW_KEY_I && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'i' : 'I';
		drawTheString();
	}
	else if (key == GLFW_KEY_J && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'j' : 'J';
		drawTheString();
	}
	else if (key == GLFW_KEY_K && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'k' : 'K';
		drawTheString();
	}
	else if (key == GLFW_KEY_L && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'l' : 'L';
		drawTheString();
	}
	else if (key == GLFW_KEY_M && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'm' : 'M';
		drawTheString();
	}
	else if (key == GLFW_KEY_N && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'n' : 'N';
		drawTheString();
	}
	else if (key == GLFW_KEY_O && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'o' : 'O';
		drawTheString();
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'p' : 'P';
		drawTheString();
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'q' : 'Q';
		drawTheString();
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'r' : 'R';
		drawTheString();
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 's' : 'S';
		drawTheString();
	}
	else if (key == GLFW_KEY_T && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 't' : 'T';
		drawTheString();
	}
	else if (key == GLFW_KEY_U && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'u' : 'U';
		drawTheString();
	}
	else if (key == GLFW_KEY_V && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'v' : 'V';
		drawTheString();
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'w' : 'W';
		drawTheString();
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'x' : 'X';
		drawTheString();
	}
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'y' : 'Y';
		drawTheString();
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? 'z' : 'Z';
		drawTheString();
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += ' ';
		drawTheString();
	}
	else if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '.';
		drawTheString();
	}
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += ',';
		drawTheString();
	}
	else if (key == GLFW_KEY_APOSTROPHE && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '\'';
		drawTheString();
	}
	else if (key == GLFW_KEY_SLASH && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += (caps == 0) ? '/' : '?';
		drawTheString();
	}
	else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '-';
		drawTheString();
	}
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		lastWorkingFont = font;
		font = myThingy;
		drawTheString();
	}
	else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS){
		if (myThingy.size() >= 1) {
			DestroyGeometry(&geometry);
			myThingy.resize(myThingy.size() - 1);
			drawTheString();
		}
	}
	else if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '0';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '1';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '2';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '3';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '4';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '5';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '6';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '7';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '8';
		drawTheString();
	}
	else if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS){
		DestroyGeometry(&geometry);
		myThingy += '9';
		drawTheString();
	}
}

int click = 0;
float previousPointX = 0.f;
float previousPointY = 0.f;
float newPointX = 0.f;
float newPointY = 0.f;

float prevX = 0.f;
float prevY = 0.f;
float newX = 0.f;
float newY = 0.f;

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		click = 1;
		newX = prevX;
		newY = prevY;

		int pointIndex = findClosePoint();
		if (pointIndex >= 0){
			previousPointX = vertices.at(pointIndex);
			previousPointY = vertices.at(pointIndex + 1);
		}
	}	
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		click = 0;
	}
}

static void CursorPosCallBack(GLFWwindow* window, double xpos, double ypos){
	prevX = ((2 * xpos) - windowWidth)/windowWidth;
	prevY = ((2 * ypos) - windowHeight)/windowHeight;

}

float radialOffset = 0.0015f;
int findClosePoint(){
	float radialDistance = sqrt(2.f * pow(radialOffset, 2.f));
	int pointIndex = -1;
	for (int i = 0; i < vertices.size(); i += 2){

		float x = 0.f;
		float y = 0.f;
		x = vertices.at(i);
		y = vertices.at(i + 1);
		float distance = sqrt(pow(abs(x - newX), 2) + pow(abs(y - newY), 2));
		if (distance <= radialDistance){
			pointIndex = i;
		}
	}
	return pointIndex;
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[]){
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
	window = glfwCreateWindow(windowWidth, windowHeight, "CPSC 453 Assignment 3", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPosCallBack);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwMakeContextCurrent(window);
	
	//Intialize GLAD if not lab linux
	#ifndef LAB_LINUX
	if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}
	#endif
	
	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	if (!InitializeShaders(&shader)) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}

	// call function to create and fill buffers with geometry data

	drawTeacup();

	glUseProgram(shader.program);
	GLint controlPointsUniform = glGetUniformLocation(shader.program, "controlPointsNumber");

	glUseProgram(shader.program);
	GLint offsetUniform = glGetUniformLocation(shader.program, "offset");

	glUseProgram(shader.program);
	GLint offsetYUniform = glGetUniformLocation(shader.program, "offsetY");

	glUseProgram(shader.program);
	GLint windowScaleUniform = glGetUniformLocation(shader.program, "windowScale");

	glUseProgram(shader.program);
	GLint spookyEffectUniform = glGetUniformLocation(shader.program, "spookyEffect");

	// run an event-triggered main loop
	while (!glfwWindowShouldClose(window))
	{
		glUseProgram(shader.program);
		glUniform1f(offsetUniform, offset);

		glUseProgram(shader.program);
		glUniform1f(offsetYUniform, offsetY);

		glUseProgram(shader.program);
		glUniform1f(windowScaleUniform, windowScale);

		glUseProgram(shader.program);
		glUniform1f(spookyEffectUniform, spookyEffect);

		offset += scale;

		offsetY += scaleY;

		if (scale < 0){
			if ((offset + totalAdvance) <= (-1.f)){
				offset = (1.5f - (startOff * windowScale));
			}
		}
		else if (scale > 0){
			if ((offset + (startOff * windowScale)) >= (1.f)){
				offset = (-1.5f) - totalAdvance;
			}
		}
		glUniform1f(controlPointsUniform, controlPoints);
		glPatchParameteri(GL_PATCH_VERTICES, controlPoints);
		// call function to draw our scene
		RenderScene(&geometry, &shader); //render scene with texture
								
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

void QueryGLVersion(){
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors(){
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
GLuint LinkProgram(GLuint vertexShader, GLuint TCSshader, GLuint TESshader, GLuint fragmentShader){
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (TCSshader) glAttachShader(programObject, TCSshader); 
	if (TESshader) glAttachShader(programObject, TESshader); 
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

void drawTeacup(){
	controlPoints = 3;
	vector<float> vertices;
	vector<float> colours;

	tempX1 = 1.f;
	tempY1 = 1.f;

	tempX2 = 2.f;
	tempY2 = -1.f;

	tempX3 = 0.f;
	tempY3 = -1.f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 6);

	vertices.push_back(1.f);
	vertices.push_back(1.f);

	vertices.push_back(2.f);
	vertices.push_back(-1.f);

	vertices.push_back(0.f);
	vertices.push_back(-1.f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 6);
		solidColor(colours, 1.f, 1.f, 0.4f, 6);
		solidColor(colours, 1.f, 1.f, 1.f, 6);

	tempX1 = 0.f;
	tempY1 = -1.f;

	tempX2 = -2.f;
	tempY2 = -1.f;

	tempX3 = -1.f;
	tempY3 = 1.f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 6);
	
	vertices.push_back(0.f);
	vertices.push_back(-1.f);
	
	vertices.push_back(-2.f);
	vertices.push_back(-1.f);

	vertices.push_back(-1.f);
	vertices.push_back(1.f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 6);
		solidColor(colours, 1.f, 1.f, 0.4f, 6);
		solidColor(colours, 1.f, 1.f, 1.f, 6);

	tempX1 = -1.f;
	tempY1 = 1.f;

	tempX2 = 0.f;
	tempY2 = 1.f;

	tempX3 = 1.f;
	tempY3 = 1.f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 6);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 6);
		solidColor(colours, 1.f, 1.f, 0.4f, 6);
		solidColor(colours, 1.f, 1.f, 1.f, 6);

	vertices.push_back(-1.f);
	vertices.push_back(1.f);

	vertices.push_back(0.f);
	vertices.push_back(1.f);

	vertices.push_back(1.f);
	vertices.push_back(1.f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 6);
		solidColor(colours, 1.f, 1.f, 1.f, 6);		
		solidColor(colours, 1.f, 1.f, 1.f, 6);

	tempX1 = 1.2f;
	tempY1 = 0.5f;

	tempX2 = 2.5f;
	tempY2 = 1.f;

	tempX3 = 1.3f;
	tempY3 = -0.4f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 6);

	vertices.push_back(1.2f);
	vertices.push_back(0.5f);

	vertices.push_back(2.5f);
	vertices.push_back(1.f);

	vertices.push_back(1.3f);
	vertices.push_back(-0.4f);
	
	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 6);
		solidColor(colours, 1.f, 1.f, 0.4f, 6);
		solidColor(colours, 1.f, 1.f, 1.f, 6);
	
	
	if (!InitializeGeometry(&geometry, vertices, colours))
		cout << "Program failed to intialize geometry!" << endl;
}

void drawFish(){
	controlPoints = 4;
	vector<float> vertices;
	vector<float> colours;

	tempX1 = 1.f;
	tempY1 = 1.f;

	tempX2 = 4.f;
	tempY2 = 0.f;

	tempX3 = 6.f;
	tempY3 = 2.f;

	tempX4 = 9.f;
	tempY4 = 1.f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);
	pushLines(tempX3, tempY3, tempX4, tempY4, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 12);

	vertices.push_back(1.f);
	vertices.push_back(1.f);

	vertices.push_back(4.f);
	vertices.push_back(0.f);

	vertices.push_back(6.f);
	vertices.push_back(2.f);

	vertices.push_back(9.f);
	vertices.push_back(1.f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 8);
		solidColor(colours, 1.f, 1.f, 0.4f, 16);
		solidColor(colours, 1.f, 1.f, 1.f, 8);



	tempX1 = 8.f;
	tempY1 = 2.f;

	tempX2 = 0.f;
	tempY2 = 8.f;

	tempX3 = 0.f;
	tempY3 = -2.f;

	tempX4 = 8.f;
	tempY4 = 4.f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);
	pushLines(tempX3, tempY3, tempX4, tempY4, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 12);

	vertices.push_back(8.f);
	vertices.push_back(2.f);

	vertices.push_back(0.f);
	vertices.push_back(8.f);

	vertices.push_back(0.f);
	vertices.push_back(-2.f);

	vertices.push_back(8.f);
	vertices.push_back(4.f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 8);
		solidColor(colours, 1.f, 1.f, 0.4f, 16);
		solidColor(colours, 1.f, 1.f, 1.f, 8);

	tempX1 = 5.f;
	tempY1 = 3.f;

	tempX2 = 3.f;
	tempY2 = 2.f;

	tempX3 = 3.f;
	tempY3 = 3.f;

	tempX4 = 5.f;
	tempY4 = 2.f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);
	pushLines(tempX3, tempY3, tempX4, tempY4, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 12);

	vertices.push_back(5.f);
	vertices.push_back(3.f);

	vertices.push_back(3.f);
	vertices.push_back(2.f);

	vertices.push_back(3.f);
	vertices.push_back(3.f);

	vertices.push_back(5.f);
	vertices.push_back(2.f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 8);
		solidColor(colours, 1.f, 1.f, 0.4f, 16);
		solidColor(colours, 1.f, 1.f, 1.f, 8);

	tempX1 = 3.f;
	tempY1 = 2.2f;

	tempX2 = 3.5f;
	tempY2 = 2.7f;

	tempX3 = 3.5f;
	tempY3 = 3.3f;

	tempX4 = 3.f;
	tempY4 = 3.8f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);
	pushLines(tempX3, tempY3, tempX4, tempY4, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 12);

	vertices.push_back(3.f);
	vertices.push_back(2.2f);

	vertices.push_back(3.5f);
	vertices.push_back(2.7f);

	vertices.push_back(3.5f);
	vertices.push_back(3.3f);

	vertices.push_back(3.f);
	vertices.push_back(3.8f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 8);
		solidColor(colours, 1.f, 1.f, 0.4f, 16);
		solidColor(colours, 1.f, 1.f, 1.f, 8);

	tempX1 = 2.8f;
	tempY1 = 3.5f;

	tempX2 = 2.4f;
	tempY2 = 3.8f;

	tempX3 = 2.4f;
	tempY3 = 3.2f;
	
	tempX4 = 2.8f;
	tempY4 = 3.5f;

	pushLines(tempX1, tempY1, tempX2, tempY2, vertices);
	pushLines(tempX2, tempY2, tempX3, tempY3, vertices);
	pushLines(tempX3, tempY3, tempX4, tempY4, vertices);

	solidColor(colours, 0.3f, 0.3f, 0.3f, 12); // grey

	vertices.push_back(2.8f);
	vertices.push_back(3.5f);

	vertices.push_back(2.4f);
	vertices.push_back(3.8f);

	vertices.push_back(2.4f);
	vertices.push_back(3.2f);

	vertices.push_back(2.8f);
	vertices.push_back(3.5f);

	bluePurple(colours);

		cross(vertices);
		solidColor(colours, 1.f, 1.f, 1.f, 8);
		solidColor(colours, 1.f, 1.f, 0.4f, 16);
		solidColor(colours, 1.f, 1.f, 1.f, 8);

	
	if (!InitializeGeometry(&geometry, vertices, colours))
		cout << "Program failed to intialize geometry!" << endl;
}

void bluePurple(vector<float>& colours){
	colours.push_back(1.0f);
	colours.push_back(0.0f);
	colours.push_back(1.0f);

	colours.push_back(0.0f);
	colours.push_back(1.0f);
	colours.push_back(1.0f);

	colours.push_back(0.0f);
	colours.push_back(1.0f);
	colours.push_back(1.0f);

	if (controlPoints == 4){
		colours.push_back(1.0f);
		colours.push_back(0.0f);
		colours.push_back(1.0f);
	}
}

void solidColor(vector<float>& colours, float r, float g, float b, int repeat){
	for (int i = 0; i < repeat; i++){
		colours.push_back(r);
		colours.push_back(g);
		colours.push_back(b);
	}
}

void arrow(vector<float>& vertices){
		pushPieces(tempX1, tempY1, vertices);
		pushPieces(tempX2, tempY2, vertices);
		pushPieces(tempX3, tempY3, vertices);
		
		if (controlPoints == 4)
			pushPieces(tempX4, tempY4, vertices);
}

void cross(vector<float>& vertices){
	pushLines(tempX1 - 0.13f, tempY1 + 0.13f, tempX1 + 0.13f, tempY1 - 0.13f, vertices);
	pushLines(tempX1 - 0.13f, tempY1 - 0.13f, tempX1 + 0.13f, tempY1 + 0.13f, vertices);

	pushLines(tempX2 - 0.13f, tempY2 + 0.13f, tempX2 + 0.13f, tempY2 - 0.13f, vertices);
	pushLines(tempX2 - 0.13f, tempY2 - 0.13f, tempX2 + 0.13f, tempY2 + 0.13f, vertices);

	pushLines(tempX3 - 0.13f, tempY3 + 0.13f, tempX3 + 0.13f, tempY3 - 0.13f, vertices);
	pushLines(tempX3 - 0.13f, tempY3 - 0.13f, tempX3 + 0.13f, tempY3 + 0.13f, vertices);

	if (controlPoints == 4){
		pushLines(tempX4 - 0.13f, tempY4 + 0.13f, tempX4 + 0.13f, tempY4 - 0.13f, vertices);
		pushLines(tempX4 - 0.13f, tempY4 - 0.13f, tempX4 + 0.13f, tempY4 + 0.13f, vertices);
	}
}

void pushPieces(float tempX, float tempY, vector<float>& vertices){
	vertices.push_back(tempX - 0.13f);
	vertices.push_back(tempY + 0.13f);

	vertices.push_back(tempX);
	vertices.push_back(tempY);

	if (controlPoints == 4){
		vertices.push_back(tempX);
		vertices.push_back(tempY);
	}

	vertices.push_back(tempX + 0.13f);
	vertices.push_back(tempY + 0.13f);
}

void pushLines(float tempX, float tempY, float tempXEnd, float tempYEnd, vector<float>& vertices){
	vertices.push_back(tempX);
	vertices.push_back(tempY);

	vertices.push_back(tempX);
	vertices.push_back(tempY);

	if (controlPoints == 4){
		vertices.push_back(tempXEnd);
		vertices.push_back(tempYEnd);
	}

	vertices.push_back(tempXEnd);
	vertices.push_back(tempYEnd);
}

float drawGlyphs(char letter, GlyphExtractor extractor, vector<float>& vertices, vector<float>& colours, float advance, float r, float g, float b){

	if (fontType.compare("ttf") == 0){
		controlPoints = 3;
	}
	else if (fontType.compare("otf") == 0){
		controlPoints = 4;
	}

	MyGlyph glyph = extractor.ExtractGlyph(letter);
	for (int i = 0; i < glyph.contours.size(); i++){
		for (int j = 0; j < glyph.contours[i].size(); j++){
			int degreeNumber = (glyph.contours[i])[j].degree;
		//	cout << "x: " << (glyph.contours[i])[j].x[0] << " y: " << (glyph.contours[i])[j].y[0] << endl;

			if (degreeNumber == 1){

				vertices.push_back((glyph.contours[i])[j].x[0] + advance);
				vertices.push_back((glyph.contours[i])[j].y[0] - (line));

				vertices.push_back((glyph.contours[i])[j].x[0] + advance);
				vertices.push_back((glyph.contours[i])[j].y[0] - (line));

				vertices.push_back((glyph.contours[i])[j].x[1] + advance);
				vertices.push_back((glyph.contours[i])[j].y[1] - (line));


				solidColor(colours, r, g, b, 3);

				if (controlPoints == 4){
					vertices.push_back((glyph.contours[i])[j].x[1] + advance);
					vertices.push_back((glyph.contours[i])[j].y[1] - (line));
					solidColor(colours, r, g, b, 1);
				}

			}
			else if (degreeNumber == 2){
				vertices.push_back((glyph.contours[i])[j].x[0] + advance);
				vertices.push_back((glyph.contours[i])[j].y[0] - (line));

				vertices.push_back((glyph.contours[i])[j].x[1] + advance);
				vertices.push_back((glyph.contours[i])[j].y[1] - (line));

				vertices.push_back((glyph.contours[i])[j].x[2] + advance);
				vertices.push_back((glyph.contours[i])[j].y[2] - (line));

				solidColor(colours, r, g, b, 3);

				if (controlPoints == 4){
					vertices.push_back((glyph.contours[i])[j].x[2] + advance);
					vertices.push_back((glyph.contours[i])[j].y[2] - (line));
					solidColor(colours, r, g, b, 1);
				}

			}
			else if (degreeNumber == 3){
				vertices.push_back((glyph.contours[i])[j].x[0] + advance);
				vertices.push_back((glyph.contours[i])[j].y[0] - (line));

				vertices.push_back((glyph.contours[i])[j].x[1] + advance);
				vertices.push_back((glyph.contours[i])[j].y[1] - (line));

				vertices.push_back((glyph.contours[i])[j].x[2] + advance);
				vertices.push_back((glyph.contours[i])[j].y[2] - (line));

				vertices.push_back((glyph.contours[i])[j].x[3] + advance);
				vertices.push_back((glyph.contours[i])[j].y[3] - (line));
			
				solidColor(colours, r, g, b, 4);
			}
		
		}
	}

	float returnAdvance = glyph.advance;
	return returnAdvance;
}

void drawString(string things){
	startOff = things.length()/2 * -0.5f * 1.1f;

	float r = 0.2f;
	float g = 1.f;
	float b = 1.f;

	float endRed = 0.384313725f;
	float endGreen = 0.082352941f;
	float endBlue = 0.807843137f;

	GlyphExtractor extractor;

	if (!extractor.LoadFontFile(font)){
		font = lastWorkingFont;
		extractor.LoadFontFile(font);
	}

	int fontStringLength = font.length();
	char temp[4];
	temp[0] = font.at(fontStringLength - 3);
	temp[1] = font.at(fontStringLength - 2);
	temp[2] = font.at(fontStringLength - 1);
	temp[3] = '\0';

	fontType = temp;

	char letter;

	float advance = startOff;

	float redChange = (r > endRed) ? r - endRed : -(endRed - r);
	float greenChange = (g > endGreen) ? g - endGreen : -(endGreen - g);
	float blueChange = (b > endBlue) ? b - endBlue : -(endBlue - b);

	previousThingsLength = thingsLength;

	thingsLength = things.length();

	for (int i = 0; i < thingsLength; i++){
		letter = things.at(i);
		advance += drawGlyphs(letter, extractor, vertices, colours, advance, r, g, b);
		
		r -= ((redChange) / things.length());
		g -= ((greenChange) / things.length());
		b -= ((blueChange) / things.length());
	}

	totalAdvance = advance * windowScale;
}

void drawTheString(){
	line = 0;
	DestroyGeometry(&geometry);
	drawString(myThingy);
	if (!InitializeGeometry(&geometry, vertices, colours))
		cout << "Program failed to intialize geometry!" << endl;

}

void drawDaDaa(){
	vector<string> tadaa;
	tadaa.push_back("Daaaaaaa ");
	tadaa.push_back("da ");
	tadaa.push_back("dadadadaaaaaaa ");
	tadaa.push_back("da ");
	tadaa.push_back("dadadadaaaaaaaa  ");
	tadaa.push_back("da ");
	tadaa.push_back("dadadadaaaaaaaaaaa");

	drawStrings(tadaa);
}

void drawStrings(vector<string> tadaa){
	line = 0;
	DestroyGeometry(&geometry);
	for (int i = 0; i < tadaa.size(); i++){
		startOff = tadaa[i].length()/2 * -0.5f * 1.1f;
		drawString(tadaa[i]);
		line += lineSize;
	}
	if (!InitializeGeometry(&geometry, vertices, colours))
		cout << "Program failed to intialize geometry!" << endl;
}