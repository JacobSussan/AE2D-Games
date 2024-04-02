#include "RainbowBrick.h"

#include "../AE2D/AEErrors.h"
#include "../AE2D/Helper.h"
#include "../BlockData.h"
#include "../Helper.h"

#include <random>

//-------------------------------------------------------------------
void RainbowBrick::init()
{
	m_blockID = BLOCK_ID::RAINBOW_BRICK;
	AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/red_bricks.png");

	std::uniform_int_distribution<int> color(0, 255);
	m_color.r = color(randomGenerator);
	m_color.g = color(randomGenerator);
	m_color.b = color(randomGenerator);
	m_color.a = 255;
}

