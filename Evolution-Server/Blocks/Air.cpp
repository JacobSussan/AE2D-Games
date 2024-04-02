#include "Air.h"

#include "../AE2D/AEErrors.h"


//-------------------------------------------------------------------
void Air_S::init()
{
	m_blockID = BLOCK_ID::AIR;
}

//-------------------------------------------------------------------
void Air_S::createCounterpart(b2World *world, std::vector<Block_S*> &blocks, const glm::vec2 &pos)
{
	AE::fatalError("Tried creating a counterpart for air...\n\n Blocks/Air.cpp Air_S::createCounterpart\n\n");
}

