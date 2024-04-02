#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "Box.h"

#include "Block.h"
#include "BlockData.h"
#include "Blocks/Air.h"
#include "Blocks/Brick.h"
#include "Blocks/LightBrick.h"
#include "Blocks/Glass.h"
#include "Blocks/RainbowBrick.h"

#include <random>
#include <ctime>

// public vars
static std::mt19937 randomGenerator(time(0));

//-------------------------------------------------------------------
class Helper
{
	public:
		static bool 
				checkBox2DPos(b2World* world, const glm::vec2 &pos);
		//-----------------------------------
		static Block_S* getBlock_SFromPos(const glm::vec2 &pos,
					std::vector< std::vector<Block_S*> > &TLBlocks,
				 	std::vector< std::vector<Block_S*> > &TRBlocks,
				 	std::vector< std::vector<Block_S*> > &BLBlocks,
				 	std::vector< std::vector<Block_S*> > &BRBlocks);
		//-----------------------------------
		static BLOCK_QUAD getBlock_SQuad(const glm::vec2 &pos);
		//-----------------------------------
		static Block_S* getBlockTypeFromID(BLOCK_ID block_ID);
};

