#version 330 core
layout(location = 0) in vec3 vertexPosModelSpace;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 MVP; // transform matrix

void main()
{
	gl_Position = MVP * vec4(vertexPosModelSpace, 1);
}