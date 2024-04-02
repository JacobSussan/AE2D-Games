#include "Helper.h"
#include "AE2D/AEErrors.h"

#include <vector>
#include <iostream>

//-------------------------------------------------------------------
//subclass b2QueryCallback
class MyQueryCallback : public b2QueryCallback 
{
	public:
		std::vector<b2Body*> foundBodies;
      
		bool ReportFixture(b2Fixture* fixture) {
			foundBodies.push_back(fixture->GetBody()); 
			return true; //keep going to find all fixtures in the query area
		}
};

//-------------------------------------------------------------------
bool Helper::checkBox2DPos(b2World* world, const glm::vec2 &pos)
{
	MyQueryCallback queryCallback;
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(round(pos.x) - 0.25, round(pos.y) - 0.25);
	aabb.upperBound = b2Vec2(round(pos.x) + 0.25, round(pos.y) + 0.25);
	world->QueryAABB( &queryCallback, aabb );
	
	if (queryCallback.foundBodies.size() > 0) {
		return true;
	} else {
		return false;
	}
}

//-------------------------------------------------------------------
BLOCK_QUAD Helper::getQuad(const glm::vec2 &pos)
{
	if (pos.y >= 0) {		 	// top
		if (pos.x >= 0) { 			// right
			return BLOCK_QUAD::TR;
		} else if (pos.x < 0) {		// left
			return BLOCK_QUAD::TL;
		}
	} else if (pos.y < 0) {		// bottom
		if (pos.x >= 0) {				// right
			return BLOCK_QUAD::BR;
		} else if (pos.x < 0 ) {		// left
			return BLOCK_QUAD::BL;
		}
	}

	return BLOCK_QUAD::NO_QUAD;
}

//-------------------------------------------------------------------
Block* Helper::getBlockTypeFromID(BLOCK_ID blockID)
{
	switch(blockID) {
		case BLOCK_ID::AIR:
			return new Air;
		break;
		//-----------------------------------
		case BLOCK_ID::BRICK:
			return new Brick;
		break;
		//-----------------------------------
		case BLOCK_ID::RAINBOW_BRICK:
			return new RainbowBrick;
		break;
		//-----------------------------------
		case BLOCK_ID::LIGHT_BRICK:
			return new LightBrick;
		break;
		//-----------------------------------
		case BLOCK_ID::GLASS:
			return new Glass;
		break;
		//-----------------------------------
		case BLOCK_ID::NO_ID:
		default:
		std::cout << "Tried loading a block with invalid id. Block ID was " <<  std::to_string((int)blockID) << std::endl;
		
		break;
	}
	return nullptr;
}

//-------------------------------------------------------------------
Block* Helper::getBlock(glm::vec2 pos, std::vector<chunk_S> &c)
{
	for (unsigned long i = 0; i < c.size(); i++) {
		if (c[i].m_pos.x == ((int)pos.x % 16) && c[i].m_pos.y == ((int)pos.y % 16)) {
			return (*c[i].m_chunk->getBlocks())[(int)pos.y][(int)pos.x];
		}
	}

	std::cout << "Unable to find block at "	<< pos.x << " " << pos.y << std::endl;
	return (*c[0].m_chunk->getBlocks())[(int)pos.y][(int)pos.x];
}

