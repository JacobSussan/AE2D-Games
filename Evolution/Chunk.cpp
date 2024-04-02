#include "Chunk.h"
#include <iostream>

//-------------------------------------------------------------------
void Chunk::activate(std::vector< std::vector<Block* > > blocks)
{
	m_blocks = blocks;
}

//-------------------------------------------------------------------
void Chunk::deactivate()
{
	// loop all blocks and remove
}

//-------------------------------------------------------------------
void Chunk::createBodies(b2World* world, glm::vec2 chunkPos)
{
	for (unsigned long i = 0; i < 16; i++) {
		for (unsigned long j = 0; j < 16; j++) {
			m_blocks[i][j]->createBody(world, glm::vec2((chunkPos.y * 16) + i, (chunkPos.x * 16) + j), true);
		}
	}
}

//-------------------------------------------------------------------
void Chunk::draw(AE::SpriteBatch spriteBatch)
{
	for (unsigned long i = 0; i < m_blocks.size(); i++) {
		for (unsigned long j =0; j < m_blocks[i].size(); j++) {
			m_blocks[i][j]->draw(spriteBatch);
		}	
	}
}

//-------------------------------------------------------------------
void Chunk::drawDebug(AE::DebugRenderer debugRenderer)
{
	for (unsigned long i = 0; i < m_blocks.size(); i++) {
		for (unsigned long j =0; j < m_blocks[i].size(); j++) {
			m_blocks[i][j]->drawDebug(debugRenderer);
		}	
	}
}

