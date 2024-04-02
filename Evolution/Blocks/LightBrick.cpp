#include "LightBrick.h"

#include "../AE2D/AEErrors.h"
#include "../BlockData.h"

//-------------------------------------------------------------------
void LightBrick::init()
{
	m_blockID = BLOCK_ID::LIGHT_BRICK;
	AE::Color_rgb8 color;
	AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/light_bricks.png");

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

}

