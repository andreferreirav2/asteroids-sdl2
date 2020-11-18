#include "SDLApp.h"
#include "../Types.h"
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

bool SDLApp::isOpenGL()
{
	return m_opengl;
}

void SDLApp::setOpenGL(bool opengl)
{
	m_opengl = opengl;
}


bool SDLApp::initGL()
{
	GLenum error = GL_NO_ERROR;

	// Define shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shaders from file
	string vertexShaderCode;
	ifstream vertexShaderStream = ifstream(m_vertexShaderPath, ios::in);
	if (vertexShaderStream.is_open())
	{
		stringstream sstr;
		sstr << vertexShaderStream.rdbuf();
		vertexShaderCode = sstr.str();
		vertexShaderStream.close();
	}
	else
	{
		cerr << "Unable to open vertex shader file: " << m_vertexShaderPath << endl;
		return false;
	}

	string fragmentShaderCode;
	ifstream fragmentShaderStream = ifstream(m_fragmentShaderPath, ios::in);
	if (fragmentShaderStream.is_open())
	{
		stringstream sstr;
		sstr << fragmentShaderStream.rdbuf();
		fragmentShaderCode = sstr.str();
		fragmentShaderStream.close();
	}
	else
	{
		cerr << "Unable to open fragment shader file: " << m_fragmentShaderPath << endl;
		return false;
	}


	GLint result = GL_FALSE;
	int InfoLogLength;

	// Compile vertex shader
	//const GLchar* vertexShaderSource = "#version 140\nin vec3 LVertexPos3D; void main() { gl_Position = vec4( LVertexPos3D.x, LVertexPos3D.y, LVertexPos3D.z, 1 ); }";
	char const* vertexShaderSource = vertexShaderCode.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//Check for vertex shader errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result != GL_TRUE)
	{
		auto vertexShaderErrorMessage = vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(vertexShader, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);
		cerr << "Unable to compile vertex shader: " << vertexShader << endl;
		cerr << &vertexShaderErrorMessage[0] << endl;
		return false;
	}

	// Compile fragment shader
	//const GLchar* fragmentShaderSource = "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }";
	char const* fragmentShaderSource = fragmentShaderCode.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//Check for fragment shader errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result != GL_TRUE)
	{
		auto fragmentShaderErrorMessage = vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(fragmentShader, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		cerr << "Unable to compile fragment shader: " << fragmentShader << endl;
		cerr << &fragmentShaderErrorMessage[0] << endl;
		return false;
	}


	// Link gl program
	m_glProgramID = glCreateProgram();
	glAttachShader(m_glProgramID, vertexShader);
	glAttachShader(m_glProgramID, fragmentShader);
	glLinkProgram(m_glProgramID);
	//Check for errors
	glGetShaderiv(m_glProgramID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(m_glProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (result != GL_TRUE)
	{
		auto programErrorMessage = vector<char>(InfoLogLength + 1);
		glGetShaderInfoLog(m_glProgramID, InfoLogLength, NULL, &programErrorMessage[0]);
		cerr << "Unable to compile gl program: " << m_glProgramID << endl;
		cerr << &programErrorMessage[0] << endl;
		return false;
	}

	//Get vertex attribute location
	m_glVertexPos3DLocation = glGetAttribLocation(m_glProgramID, "vertexPosModelSpace");
	if (m_glVertexPos3DLocation == -1)
	{
		cerr << "vertexPosModelSpace is not a valid glsl program variable!" << endl;
		return false;
	}


	glDetachShader(m_glProgramID, vertexShader);
	glDetachShader(m_glProgramID, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}


void SDLApp::setClearColorGL(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void SDLApp::setBuffersData()
{
	//Vertex Buffer Obj data
	GLfloat vertexData[] =
	{
		0.0f,  -0.3f, 0.0f,
		-0.3f, -0.5f, 0.0f,
		 0.0f, 0.7f, 0.0f,
		0.3f, -0.5f, 0.0f,
	};
	//Index Buffer Obj data
	GLuint indexData[] = { 0, 1, 2, 3 };

	//Create Vertex Buffer Obj
	glGenBuffers(1, &m_glVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	//Create Index Buffer Obj
	glGenBuffers(1, &m_glIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
}


void SDLApp::renderGL()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind program
	glUseProgram(m_glProgramID);

	//Enable vertex position
	glEnableVertexAttribArray(m_glVertexPos3DLocation);

	//Set vertex / index data
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glVertexAttribPointer(m_glVertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);

	// Draw
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	//Disable vertex position
	glDisableVertexAttribArray(m_glVertexPos3DLocation);

	//Unbind program
	glUseProgram(NULL);
}

void SDLApp::presentGL()
{
	SDL_GL_SwapWindow(m_window.get());
}

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::ortho(0.f, 400.f, 0.f, 400.f, -1.f, 1.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}