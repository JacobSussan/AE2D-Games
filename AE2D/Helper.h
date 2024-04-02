#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <CEGUI/CEGUI.h>
#include <SDL2/SDL_events.h>

#include <random>
#include <ctime>
namespace AE {
//-------------------------------------------------------------------
class Helper
{
	public:
		static CEGUI::Key::Scan 
				SDLKeyToCEGUIKey(SDL_Keycode key);
		//-----------------------------------
		static CEGUI::MouseButton 
				SDLButtonToCEGUIButton(Uint8 sdlButton);
		//-----------------------------------
		
};
}

