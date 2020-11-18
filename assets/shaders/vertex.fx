#version 330 core
layout(location = 0) in vec3 vertexPosModelSpace;

void main()
{
	gl_Position.xyz = vertexPosModelSpace;
	gl_Position.w = 1.0;
}