#include "SDLApp.h"
#include "../Types.h"

void SDLApp::setClearColorGL(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void SDLApp::setBuffersData()
{
	//Vertex Buffer Obj data
	GLfloat vertexData[] =
	{
		0.0f,  -0.3f,
		-0.3f, -0.5f,
		 0.0f, 0.7f,
		0.3f, -0.5f,
	};
	//Index Buffer Obj data
	GLuint indexData[] = { 0, 1, 2, 3 };

	//Create Vertex Buffer Obj
	glGenBuffers(1, &m_glVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
	//Create Index Buffer Obj
	glGenBuffers(1, &m_glIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
}


void SDLApp::renderGL()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Bind program
	glUseProgram(m_glProgramID);

	//Enable vertex position
	glEnableVertexAttribArray(m_glVertexPos2DLocation);

	//Set vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_glVBO);
	glVertexAttribPointer(m_glVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	//Set index data and render
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	//Disable vertex position
	glDisableVertexAttribArray(m_glVertexPos2DLocation);

	//Unbind program
	glUseProgram(NULL);
}

void SDLApp::presentGL()
{
	SDL_GL_SwapWindow(m_window.get());
}