#include "SDLApp.h"
#include "../Types.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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

	//Get vertex attribute location
	m_glMatrix = glGetUniformLocation(m_glProgramID, "MVP");
	if (m_glMatrix == -1)
	{
		cerr << "MVP is not a valid glsl program variable!" << endl;
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
	/*
	//Vertex Buffer Obj data
	GLfloat vertexData[] =
	{
		0.0f,  -0.3f, 0.1f, // middle high
		-0.4f, -0.5f, 0.0f, //left
		 0.0f, 0.7f, 0.0f, // tip
		0.4f, -0.5f, 0.0f, // right
		0.0f,  -0.3f, 0.1f, // middle low
	};
	//Index Buffer Obj data
	GLuint indexData[] = {
		0, 1, 2, // top left wing
		0, 2, 3, // top right wing
		4, 2, 1, // bottom left wing
		4, 3, 2, // bottom right wing
	};

	//Create Vertex Buffer Obj
	glGenBuffers(1, &m_glVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	//Create Index Buffer Obj
	glGenBuffers(1, &m_glIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);
	*/


	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<GLushort> faceElements;
	loadObjFile("assets/models/asteroid OBJ.obj", vertices, normals, faceElements);


	glGenBuffers(1, &m_glVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	//glGenBuffers(1, &m_glVBONormals);
	//glBindBuffer(GL_ARRAY_BUFFER, m_glVBONormals);
	//glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_glIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceElements.size() * sizeof(GLushort), faceElements.data(), GL_STATIC_DRAW);


}

glm::mat4 camera()
{
	// Orthographic camera
	// TODO: replace 400s and 300s with world coordinates
	glm::mat4 proj = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, 0.0f, 1000.0f); // In world coordinates

	// Camera matrix
	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 0, 100), // Camera is at (0,0,100), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up
	);
	return proj * view;
}

void SDLApp::renderGL(float x, float y, float rotate)
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind program
	glUseProgram(m_glProgramID);


	//Enable vertex position
	glEnableVertexAttribArray(m_glVertexPos3DLocation);

	//Set vertex / index data
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glVertexAttribPointer(m_glVertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);

	// Draw
	// TODO: replace 400s and 300s with world coordinates
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x - 400, -y + 300, 0.0f));
	model = glm::rotate(model, glm::radians(rotate - 90), glm::vec3(1.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	glm::mat4 mvp = camera() * model;

	glUniformMatrix4fv(m_glMatrix, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, 35640, GL_UNSIGNED_SHORT, NULL);

	//Disable vertex position
	glDisableVertexAttribArray(m_glVertexPos3DLocation);

	//Unbind program
	glUseProgram(NULL);
}

void SDLApp::presentGL()
{
	SDL_GL_SwapWindow(m_window.get());
}

bool SDLApp::loadObjFile(string const& objPath, vector<glm::vec3>& vertices, vector<glm::vec3>& normals, vector<GLushort>& faceElements)
{
	ifstream in = ifstream(objPath.c_str(), ios::in);
	if (!in)
	{
		cerr << "Unable to open obj file: " << objPath << endl;
		return false;
	}

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ") // Load vertices
		{
			istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ") // Load faceElements (vertex indexes)
		{
			istringstream s(line.substr(2));
			string sa, sb, sc;
			s >> sa;
			s >> sb;
			s >> sc;
			sa = sa.substr(0, sa.find("/"));
			sb = sb.substr(0, sb.find("/"));
			sc = sc.substr(0, sc.find("/"));
			GLushort a, b, c;
			istringstream(sa) >> a;
			istringstream(sb) >> b;
			istringstream(sc) >> c;
			faceElements.push_back(a - 1); faceElements.push_back(b - 1); faceElements.push_back(c - 1);
		}
		else if (line.substr(0, 2) == "vn") // Vertex normals
		{
			// ignore for now
		}
		else if (line.substr(0, 2) == "vt") // Vertex texture
		{
			// ignore for now
		}
		else if (line[0] == '#')
		{
			// ignore for now
		}
		else
		{
			// ignore for now
		}
	}

	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < faceElements.size(); i += 3)
	{
		GLushort ia = faceElements[i];
		GLushort ib = faceElements[i + 1];
		GLushort ic = faceElements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
		normals[ia] = normals[ib] = normals[ic] = normal;
	}
}
