#include "Brick.h"

#include "../AE2D/AEErrors.h"
#include "../BlockData.h"

//-------------------------------------------------------------------
void Brick::init()
{
	m_blockID = BLOCK_ID::BRICK;
	m_texture = AE::ResourceManager::getTexture("Textures/red_bricks.png");

	m_color.r = 255;
	m_color.g = 255;
	m_color.b = 255;
	m_color.a = 255;
	m_dimensions = glm::vec2(1,1);
}

