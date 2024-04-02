#pragma once

#include "Helper.h"
#include "Block.h"

class Chunk;

//-------------------------------------------------------------------
struct chunk_S
{
	public:
	Chunk* m_chunk;
	glm::vec2 m_pos;
};

//-------------------------------------------------------------------
class Chunk 
{
public:
	void activate(std::vector< std::vector<Block* > > blocks);
	void deactivate();
	void createBodies(b2World* world, glm::vec2 chunkPos);
	void draw(AE::SpriteBatch spriteBatch);
	void drawDebug(AE::DebugRenderer debugRenderer);

	// getters
	std::vector< std::vector<Block* > > *getBlocks() {
		return &m_blocks;
	}
	
	BLOCK_QUAD getQuad() {
		return m_chunkQuad;
	} 

	// setters
	void setBlock(Block *block, int x, int y) {
		m_blocks.at(y).at(x) = block;
	}

	void setQuad(BLOCK_QUAD q) {
		m_chunkQuad = q;
	}

private:
	std::vector< std::vector<Block* > > m_blocks;
	BLOCK_QUAD m_chunkQuad = BLOCK_QUAD::NO_QUAD;
};


