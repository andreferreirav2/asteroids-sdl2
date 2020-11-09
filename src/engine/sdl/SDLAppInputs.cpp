#include "SDLApp.h"
#include "../input/Inputs.h"
#include <memory>
#include <set>

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
		else if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				keys.insert(Key::KEY_UP);
				break;
			case SDLK_DOWN:
				keys.insert(Key::KEY_DOWN);
				break;
			case SDLK_LEFT:
				keys.insert(Key::KEY_LEFT);
				break;
			case SDLK_RIGHT:
				keys.insert(Key::KEY_RIGHT);
				break;
			case SDLK_SPACE:
				keys.insert(Key::KEY_SPACE);
				break;
			}
		}
	}

	/*
	Uint8 const* keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDLK_ESCAPE])
		keys.insert(Key::QUIT);

	if (keystates[SDLK_UP])
		keys.insert(Key::KEY_UP);
	if (keystates[SDLK_DOWN])
		keys.insert(Key::KEY_DOWN);
	if (keystates[SDLK_LEFT])
		keys.insert(Key::KEY_LEFT);
	if (keystates[SDLK_RIGHT])
		keys.insert(Key::KEY_RIGHT);
	if (keystates[SDLK_SPACE])
		keys.insert(Key::KEY_SPACE);
	*/

	m_inputs->update(keys);
	return m_inputs;
}