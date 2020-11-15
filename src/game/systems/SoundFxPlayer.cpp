#include "SoundFxPlayer.h"
#include "../components/SoundFXSDL.h"
#include "../components/Weapon.h"
#include <cmath>


SoundFxPlayer::SoundFxPlayer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SoundFxPlayer::onStart(ECSManager& manager)
{
	for (auto& soundFX : manager.getAllComponentsOfType<SoundFXSDL>())
	{
		soundFX->soundFX = m_sdlApp.getSoundFX(soundFX->path); // Reuse same loaded sound for all usages, by path
	}
}

void SoundFxPlayer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<SoundFXSDL>())
	{
		auto soundFX = manager.getComponentOfType<SoundFXSDL>(e);
		auto weapon = manager.getComponentOfType<Weapon>(e);

		if (weapon && weapon->shooting)
		{
			m_sdlApp.playSoundFX(soundFX->soundFX);
		}
	}
}
