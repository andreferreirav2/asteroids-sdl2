#include "SDLApp.h"
#include <SDL_opengl.h>
#include <GL\GLU.h>

void SDLApp::renderGL()
{
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
}

void SDLApp::presentGL()
{
	SDL_GL_SwapWindow(m_window.get());
}