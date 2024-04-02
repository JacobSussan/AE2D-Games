#include "Glass.h"

#include "../AE2D/AEErrors.h"
#include "../BlockData.h"

//-------------------------------------------------------------------
void Glass::init() 
{
	m_blockID = BLOCK_ID::GLASS;
	AE::Color_rgb8 color;
	static AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/glass.png");
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 42;
}

