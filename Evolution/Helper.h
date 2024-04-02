#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

#include "Chunk.h"
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

struct chunk_S;

// global public vars
static std::mt19937 randomGenerator(time(0));
static bool isServer = false;
#define CLIENT

//-------------------------------------------------------------------
class Helper
{
	public:
		static bool checkBox2DPos(b2World* world, 
						const glm::vec2 &pos);
		//-----------------------------------
		static BLOCK_QUAD getQuad(const glm::vec2 &pos);
		//-----------------------------------
		static Block* getBlockTypeFromID(BLOCK_ID block_ID);
		//-----------------------------------
		static Block* getBlock(glm::vec2 pos, 
						std::vector<chunk_S> &c);
};

