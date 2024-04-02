#include "RainbowBrick.h"

#include "../AE2D/AEErrors.h"
#include "../Helper.h"
#include "../BlockData.h"

#include <random>

//-------------------------------------------------------------------
void RainbowBrick_S::init()
{
	m_blockID = BLOCK_ID::RAINBOW_BRICK;

	std::uniform_int_distribution<int> color(0, 255);
	m_color.r = color(randomGenerator);
	m_color.g = color(randomGenerator);
	m_color.b = color(randomGenerator);
	m_color.a = 255;
}

//-------------------------------------------------------------------
void RainbowBrick_S::createCounterpart(b2World *world, std::vector<Block_S*> &blocks, const glm::vec2 &pos)
{
	Block newBlock;
	AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/red_bricks.png");

	newBlock.init(world, pos, glm::vec2(1), texture, m_color, true);
	blocks.push_back(this);
	m_block = newBlock;
	m_hasCounterpart = true;
}

