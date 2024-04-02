#include "Block.h"

#include "Helper.h"
#include "AE2D/AEErrors.h"

#include <iostream>

//-------------------------------------------------------------------
void Block::init(
				b2World* world,
				const glm::vec2& pos,
				const glm::vec2& dimentions,
				AE::GLTexture& texture,
				const AE::Color_rgb8 color,
				bool fixedRotation,
				glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) */)
				//-----------------------------------
{
	m_dimensions = dimentions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
	m_pos.x = round(pos.x);
	m_pos.y = round(pos.y);
	
	//-----------------------------------
	// Make body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(m_pos.x, m_pos.y);
	bodyDef.fixedRotation = fixedRotation;
	m_body = world->CreateBody(&bodyDef);

	b2EdgeShape edgeShape1, 
				edgeShape2, 
				edgeShape3, 
				edgeShape4,
				edgeShape5;	
	// top line
	edgeShape1.Set(b2Vec2(-0.5, 0.5), b2Vec2(0.5, 0.5));
	// left line
	edgeShape2.Set(b2Vec2(-0.5, 0.49), b2Vec2(-0.5, -0.5));
	// right line
	edgeShape3.Set(b2Vec2(0.5, -0.5), b2Vec2(0.5, 0.49));
	// bottom line
	edgeShape4.Set(b2Vec2(0.5, -0.5), b2Vec2(-0.5, -0.5));
	// line in center (for detection)
	edgeShape5.Set(b2Vec2(0.1, 0.1), b2Vec2(-0.1, -0.1));

	m_fixture1 = m_body->CreateFixture(&edgeShape1, 0);
	m_fixture2 = m_body->CreateFixture(&edgeShape2, 0);
	m_fixture3 = m_body->CreateFixture(&edgeShape3, 0);
	m_fixture4 = m_body->CreateFixture(&edgeShape4, 0);
	m_fixture5 = m_body->CreateFixture(&edgeShape5, 0);
}

//-------------------------------------------------------------------
void Block::destroy(std::vector< std::vector<Block_S*> > &TLBlocks,
					std::vector< std::vector<Block_S*> > &TRBlocks,
					std::vector< std::vector<Block_S*> > &BLBlocks,
					std::vector< std::vector<Block_S*> > &BRBlocks)
{
	// destroy m_body and set it to nullptr
	m_body->GetWorld()->DestroyBody(m_body);
	m_body = nullptr;
	
	// set has counterpart to false and
	// set Block_S block pointer to nullptr	
	
    BLOCK_QUAD q = Helper::getBlock_SQuad(glm::vec2(m_pos.x, m_pos.y));
    switch (q) {
		case BLOCK_QUAD::TR:
			TRBlocks[m_pos.y][m_pos.x]->setCounterpart(false);
		break;
		//-----------------------------------
		case BLOCK_QUAD::TL:
			TLBlocks[m_pos.y][-m_pos.x]->setCounterpart(false);
		break;
		//-----------------------------------
		case BLOCK_QUAD::BR:
			BRBlocks[-m_pos.y][m_pos.x]->setCounterpart(false);
		break;
		//-----------------------------------
		case BLOCK_QUAD::BL:
			BLBlocks[-m_pos.y][-m_pos.x]->setCounterpart(false);
		break;
		//-----------------------------------
		case BLOCK_QUAD::NO_QUAD:
			AE::fatalError("Tried destroying a block and couldn't find the correct quad.");
		break;
	}
}

//-------------------------------------------------------------------
void Block::updateVertex(std::vector< std::vector<Block_S*> > &TLBlocks,
				 		 std::vector< std::vector<Block_S*> > &TRBlocks,
				 		 std::vector< std::vector<Block_S*> > &BLBlocks,
				 		 std::vector< std::vector<Block_S*> > &BRBlocks)
{
	b2EdgeShape edgeShape1;
	// delete the fixture
	m_body->DestroyFixture(m_fixture1);
	// create the edge shape
	edgeShape1.Set(b2Vec2(-0.5, 0.5), b2Vec2(0.5, 0.5));

	// check either side of the block, if there are other blocks there, add a vertex0 or vertex3 to this block
	Block_S* block_SToCheck;
	// check if there is a block to the left	
	block_SToCheck = Helper::getBlock_SFromPos(glm::vec2(m_pos.x - 1, m_pos.y), TLBlocks, TRBlocks, BLBlocks, BRBlocks);
	if (block_SToCheck->getBlockID() != BLOCK_ID::NO_ID && block_SToCheck->getBlockID() != BLOCK_ID::AIR) {
		// create and set vertex 0
		edgeShape1.m_vertex0.Set(-1.5, 0.5);
		edgeShape1.m_hasVertex0 = true;
	} else {
		edgeShape1.m_hasVertex0 = false;
	}
	// check if there is a block to the right
	block_SToCheck = Helper::getBlock_SFromPos(glm::vec2(m_pos.x + 1, m_pos.y), TLBlocks, TRBlocks, BLBlocks, BRBlocks);
	if (block_SToCheck->getBlockID() != BLOCK_ID::NO_ID && block_SToCheck->getBlockID() != BLOCK_ID::AIR) {
		// create and set vertex 3
		edgeShape1.m_vertex3.Set(1.5, 0.5);
		edgeShape1.m_hasVertex3 = true;
	} else {
		edgeShape1.m_hasVertex3 = false;
	}
	// recreate the fixture
	m_fixture1 = m_body->CreateFixture(&edgeShape1, 0);
}

//-------------------------------------------------------------------
// Block: blocks on the screen. Block_S: blocks off the screen
//-------------------------------------------------------------------
Block_S::~Block_S() { }
//-------------------------------------------------------------------
void Block_S::update(	std::vector< std::vector<Block_S*> > &TLBlocks,
				 		std::vector< std::vector<Block_S*> > &TRBlocks,
				 		std::vector< std::vector<Block_S*> > &BLBlocks,
				 		std::vector< std::vector<Block_S*> > &BRBlocks)
{
	// update the block in any way here
	m_block.updateVertex(TLBlocks, TRBlocks, BLBlocks, BRBlocks);
}


