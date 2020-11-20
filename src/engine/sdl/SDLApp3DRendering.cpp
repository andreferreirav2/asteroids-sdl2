#include "SDLApp.h"
#include "LoadedObj.h"
#include "../Types.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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

	m_glShaderVertexPos = glGetAttribLocation(m_glProgramID, "vertexPos");
	if (m_glShaderVertexPos == -1)
	{
		cerr << "vertexPos is not a valid glsl variable or it's not being used!" << endl;
		//return false;
	}

	m_glShaderVertexNormal = glGetAttribLocation(m_glProgramID, "vertexNormal");
	if (m_glShaderVertexNormal == -1)
	{
		cerr << "vertexNormal is not a valid glsl variable or it's not being used!" << endl;
		//return false;
	}

	m_glShaderMaterialDiffuse = glGetUniformLocation(m_glProgramID, "materialDiffuse");
	if (m_glShaderMaterialDiffuse == -1)
	{
		cerr << "materialDiffuse is not a valid glsl variable or it's not being used!" << endl;
		//return false;
	}

	m_glShaderMaterialEmissiveness = glGetUniformLocation(m_glProgramID, "materialEmissiveness");
	if (m_glShaderMaterialEmissiveness == -1)
	{
		cerr << "materialEmissiveness is not a valid glsl variable or it's not being used!" << endl;
		//return false;
	}

	m_glShaderMVP = glGetUniformLocation(m_glProgramID, "mvp");
	if (m_glShaderMVP == -1)
	{
		cerr << "mvp is not a valid glsl variable or it's not being used!" << endl;
		//return false;
	}

	m_glShaderM3x3InvTransp = glGetUniformLocation(m_glProgramID, "m3x3InvTransp");
	if (m_glShaderM3x3InvTransp == -1)
	{
		cerr << "m3x3InvTransp is not a valid glsl variable or it's not being used!" << endl;
		//return false;
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


void SDLApp::bufferObjDataGL(shared_ptr<LoadedObj> obj)
{
	glGenBuffers(1, &obj->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
	glBufferData(GL_ARRAY_BUFFER, obj->vertices.size() * sizeof(glm::vec3), obj->vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &obj->vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vboNormals);
	glBufferData(GL_ARRAY_BUFFER, obj->normals.size() * sizeof(glm::vec3), obj->normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &obj->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->faceElements.size() * sizeof(GLushort), obj->faceElements.data(), GL_STATIC_DRAW);
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

void SDLApp::renderObjGL(shared_ptr<LoadedObj> obj, glm::vec3 translate, float rotateAngle,
	glm::vec3 rotation, glm::vec3 scale, glm::vec3 colorDiffuse, float emissiveness)
{
	//Bind program
	glUseProgram(m_glProgramID);

	glEnableVertexAttribArray(m_glShaderVertexPos);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
	glVertexAttribPointer(m_glShaderVertexPos, 3, GL_FLOAT, GL_FALSE, NULL, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ibo);

	glEnableVertexAttribArray(m_glShaderVertexNormal);
	glBindBuffer(GL_ARRAY_BUFFER, obj->vboNormals);
	glVertexAttribPointer(m_glShaderVertexNormal, 3, GL_FLOAT, GL_FALSE, NULL, NULL);

	// Draw
	glUniform3fv(m_glShaderMaterialDiffuse, 1, glm::value_ptr(colorDiffuse));
	glUniform1f(m_glShaderMaterialEmissiveness, emissiveness);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, translate);
	model = glm::rotate(model, glm::radians(rotateAngle), rotation);
	model = glm::scale(model, scale);

	glm::mat4 mvp = camera() * model;

	glUniformMatrix4fv(m_glShaderMVP, 1, GL_FALSE, glm::value_ptr(mvp));

	glm::mat3 m3x3InvTransp = glm::transpose(glm::inverse(glm::mat3(model)));
	glUniformMatrix3fv(m_glShaderM3x3InvTransp, 1, GL_FALSE, glm::value_ptr(m3x3InvTransp));

	glDrawElements(GL_TRIANGLES, obj->faceElements.size(), GL_UNSIGNED_SHORT, 0);

	//Disable attributes
	glDisableVertexAttribArray(m_glShaderVertexNormal);
	glDisableVertexAttribArray(m_glShaderVertexPos);

	//Unbind program
	glUseProgram(NULL);
}

void SDLApp::clearGL()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SDLApp::presentGL()
{
	SDL_GL_SwapWindow(m_window.get());
}

shared_ptr<LoadedObj> SDLApp::loadObjFileGL(string const& objPath)
{
	auto it = m_objCache.find(objPath);
	if (it != m_objCache.end() && it->second)
	{
		return it->second;
	}

	ifstream in = ifstream(objPath.c_str(), ios::in);
	if (!in)
	{
		cerr << "Unable to open obj file: " << objPath << endl;
		return nullptr;
	}

	auto loadedObj = make_shared<LoadedObj>(objPath);

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ") // Load vertices
		{
			istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			loadedObj->vertices.push_back(v);
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
			loadedObj->faceElements.push_back(a - 1); loadedObj->faceElements.push_back(b - 1); loadedObj->faceElements.push_back(c - 1);
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

	loadedObj->normals.resize(loadedObj->vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < loadedObj->faceElements.size(); i += 3)
	{
		GLushort ia = loadedObj->faceElements[i];
		GLushort ib = loadedObj->faceElements[i + 1];
		GLushort ic = loadedObj->faceElements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(loadedObj->vertices[ib]) - glm::vec3(loadedObj->vertices[ia]),
			glm::vec3(loadedObj->vertices[ic]) - glm::vec3(loadedObj->vertices[ia])));
		loadedObj->normals[ia] = loadedObj->normals[ib] = loadedObj->normals[ic] = normal;
	}

	bufferObjDataGL(loadedObj);

	m_objCache.insert_or_assign(objPath, loadedObj);
	return loadedObj;
}
