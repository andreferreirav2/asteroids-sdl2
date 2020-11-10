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
		m_inputs->setControllerAxis(
			float2({
				SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f,
				SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f }
				),
			float2({
				SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX) / 32768.0f,
				SDL_GameControllerGetAxis(m_controller.get(), SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY) / 32768.0f }));
	}

	return m_inputs;
}


void SDLApp::hapticRumble(float strength, unsigned int duration)
{
	if (SDL_HapticRumblePlay(m_haptic.get(), strength, duration) != 0)
	{
		std::cerr << "Warning: Unable to play rumble! SDL Error: " << SDL_GetError() << std::endl;
	};
}
