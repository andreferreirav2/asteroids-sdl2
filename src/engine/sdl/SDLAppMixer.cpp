#include "SDLApp.h"
#include "SDLResourceDeleters.h"
#include <SDL_mixer.h>
#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::shared_ptr;

std::shared_ptr<Mix_Music> SDLApp::getMusic(std::string const& path)
{
	auto it = m_musicCache.find(path);
	if (it != m_musicCache.end()) // if not yet loaded, load it now
	{
		return it->second;
	}

	shared_ptr<Mix_Music> music = shared_ptr<Mix_Music>(Mix_LoadMUS(path.c_str()), MixMusicDeleter());
	if (music == NULL)
	{
		cerr << "Unable to load music " << path << "! SDL Error: " << Mix_GetError() << endl;
		return nullptr;
	}

	m_musicCache.insert_or_assign(path, music);
	return music;
}

std::shared_ptr<Mix_Chunk> SDLApp::getSoundFX(std::string const& path)
{
	auto it = m_soundFXCache.find(path);
	if (it != m_soundFXCache.end()) // if not yet loaded, load it now
	{
		return it->second;
	}

	shared_ptr<Mix_Chunk> soundFX = shared_ptr<Mix_Chunk>(Mix_LoadWAV(path.c_str()), MixChunkDeleter());
	if (soundFX == NULL)
	{
		cerr << "Unable to load soundFX " << path << "! SDL Error: " << Mix_GetError() << endl;
		return nullptr;
	}

	m_soundFXCache.insert_or_assign(path, soundFX);
	return soundFX;
}

void SDLApp::playMusic(std::shared_ptr<Mix_Music> music)
{
	Mix_PlayMusic(music.get(), -1);
}

void SDLApp::playSoundFX(std::shared_ptr<Mix_Chunk> soundFX)
{
	Mix_PlayChannel(-1, soundFX.get(), 0);
}

/*
if( Mix_PlayingMusic() == 0 )
{
	//Play the music
	Mix_PlayMusic( gMusic, -1 );
}
//If music is being played
else
{
	//If the music is paused
	if( Mix_PausedMusic() == 1 )
	{
		//Resume the music
		Mix_ResumeMusic();
	}
	//If the music is playing
	else
	{
		//Pause the music
		Mix_PauseMusic();
	}
}
break;

case SDLK_0:
//Stop the music
Mix_HaltMusic();
break;
*/