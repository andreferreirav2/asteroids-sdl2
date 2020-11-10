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
	return m_inputs;
}