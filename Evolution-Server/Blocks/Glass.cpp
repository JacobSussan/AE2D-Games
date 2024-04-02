#include "Glass.h"

#include "../AE2D/AEErrors.h"
#include "../BlockData.h"

//-------------------------------------------------------------------
void Glass_S::init()
{
	m_blockID = BLOCK_ID::BRICK;
}

//-------------------------------------------------------------------
void Glass_S::createCounterpart(b2World *world, std::vector<Block_S*> &blocks, const glm::vec2 &pos)
{
	Block newBlock;
	AE::Color_rgb8 color;
	AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/glass.png");

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 42;

	newBlock.init(world, pos, glm::vec2(1), texture, color, true);
	blocks.push_back(this);
	m_block = newBlock;
	m_hasCounterpart = true;
}

