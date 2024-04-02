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

Block_S* Helper::getBlock_SFromPos(const glm::vec2 &pos, 
							std::vector< std::vector<Block_S*> > &TLBlocks,
				 			std::vector< std::vector<Block_S*> > &TRBlocks,
				 			std::vector< std::vector<Block_S*> > &BLBlocks,
				 			std::vector< std::vector<Block_S*> > &BRBlocks)
{
	BLOCK_QUAD q = getBlock_SQuad(pos);
	
	if (q == BLOCK_QUAD::TR) {
		return TRBlocks[pos.y][pos.x];
	} else if (q == BLOCK_QUAD::TL) {
		return TLBlocks[pos.y][-pos.x];
	} else if (q == BLOCK_QUAD::BR) {
		return BRBlocks[-pos.y][pos.x];
	} else if (q == BLOCK_QUAD::BL) {
		return BLBlocks[-pos.y][-pos.x];
	}
	return nullptr;
}

//-------------------------------------------------------------------
BLOCK_QUAD Helper::getBlock_SQuad(const glm::vec2 &pos)
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

Block_S* Helper::getBlockTypeFromID(BLOCK_ID blockID)
{
	switch(blockID) {
		case BLOCK_ID::GLASS:
			return new Glass_S;
		break;
		//-----------------------------------
		case BLOCK_ID::BRICK:
			return new Brick_S;
		break;
		//-----------------------------------
		case BLOCK_ID::RAINBOW_BRICK:
			return new RainbowBrick_S;
		break;
		//-----------------------------------
		case BLOCK_ID::LIGHT_BRICK:
			return new LightBrick_S;
		break;
		//-----------------------------------
		case BLOCK_ID::AIR:
			return new Air_S;	
		break;
		//-----------------------------------
		case BLOCK_ID::NO_ID:
		default:
			AE::fatalError("Tried loading a block with no id.");
		break;
	}
	return nullptr;
}

