#pragma once
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <glm/vec3.hpp>
#include <vector>
#include <memory>
#include <string>

class LoadedObj
{
public:
	LoadedObj(std::string key, glm::vec3 colorDiffuse = { 1.0f, 1.0f, 1.0f }) : key(key) {};

	std::string key;

	GLuint vbo = 0;
	GLuint vboNormals = 0;
	GLuint ibo = 0;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> faceElements;
};

