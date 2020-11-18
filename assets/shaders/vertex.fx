#version 330 core
layout(location = 0) in vec3 vertexPosModelSpace;

uniform mat4 MVP; // transform matrix

void main()
{
	gl_Position = MVP * vec4(vertexPosModelSpace, 1);
}