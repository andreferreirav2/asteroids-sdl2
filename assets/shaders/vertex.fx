#version 140
in vec3 LVertexPos3D;

void main()
{
	gl_Position = vec4(LVertexPos3D.x, LVertexPos3D.y, LVertexPos3D.z, 1);
}