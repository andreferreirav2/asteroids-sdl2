#include "SDLApp.h"
#include "../input/Inputs.h"
#include <memory>
#include <set>
#include <iostream>

using std::set;
using std::shared_ptr;


std::shared_ptr<Inputs> SDLApp::parseInputs()
{
	SDL_Event e;
	set<Key> keys = {};

	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			keys.insert(Key::QUIT);
		}
	}

	//Set texture based on current keystate
	Uint8 const* keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDLK_ESCAPE])
		keys.insert(Key::QUIT);
	if (keystates[SDL_SCANCODE_UP])
		keys.insert(Key::KEY_UP);
	if (keystates[SDL_SCANCODE_DOWN])
		keys.insert(Key::KEY_DOWN);
	if (keystates[SDL_SCANCODE_LEFT])
		keys.insert(Key::KEY_LEFT);
	if (keystates[SDL_SCANCODE_RIGHT])
		keys.insert(Key::KEY_RIGHT);
	if (keystates[SDL_SCANCODE_SPACE])
		keys.insert(Key::KEY_SPACE);

	m_inputs->update(keys);

	// Parse controller inputs
	if (m_controller != nullptr)
	{
		float leftX = SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f;
		float leftY = SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f;

		if (abs(leftX) < 0.01f)
		{
			leftX = 0.0f;
		}
		if (abs(leftY) < 0.01f)
		{
			leftY = 0.0f;
		}

		float rightX = SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) / 32768.0f;
		float rightY = SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY) / 32768.0f;

		if (abs(rightX) < 0.01f)
		{
			rightX = 0.0f;
		}
		if (abs(rightY) < 0.01f)
		{
			rightY = 0.0f;
		}

		m_inputs->setControllerAxis(float2({ leftX, leftY }), float2({ rightX, rightY }));
	}

	return m_inputs;
}