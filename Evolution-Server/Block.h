#pragma once

#include "BlockData.h"

#include "AE2D/ResourceManager.h"
#include "AE2D/GLTexture.h"
#include "AE2D/Vertex.h"

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <random>

enum class BLOCK_QUAD { TL, TR, BL, BR, NO_QUAD };

class Block_S;
class Brick_S;

//-------------------------------------------------------------------
class Block
{
public:
	void init(
		b2World* world, 
		const glm::vec2& pos, 
		const glm::vec2& dimentions,
		AE::GLTexture& texture,
		const AE::Color_rgb8 color,
		bool fixedRotation,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//-----------------------------------
	void destroy(std::vector< std::vector<Block_S*> > &TLBlocks,
				 std::vector< std::vector<Block_S*> > &TRBlocks,
				 std::vector< std::vector<Block_S*> > &BLBlocks,
				 std::vector< std::vector<Block_S*> > &BRBlocks);
	//-----------------------------------
	void update(	std::vector< std::vector<Block_S*> > &TLBlocks,
				 	std::vector< std::vector<Block_S*> > &TRBlocks,
				 	std::vector< std::vector<Block_S*> > &BLBlocks,
				 	std::vector< std::vector<Block_S*> > &BRBlocks);
	//-----------------------------------
	void updateVertex(std::vector< std::vector<Block_S*> > &TLBlocks,
				 	std::vector< std::vector<Block_S*> > &TRBlocks,
				 	std::vector< std::vector<Block_S*> > &BLBlocks,
				 	std::vector< std::vector<Block_S*> > &BRBlocks);
	//-----------------------------------
	b2Body* 		getBody()       { return m_body; }
	glm::vec2&		getDimentions() { return m_dimensions; }
	AE::Color_rgb8& getColor() 		{ return m_color; }
	glm::vec2 		getPos() 		{ return m_pos; }
//-------------------------------------------------------------------
private:
	/* data */	
	glm::vec4 m_uvRect;
	b2Body *m_body = nullptr;
	glm::vec2 m_dimensions;
	AE::Color_rgb8 m_color;
	AE::GLTexture m_texture;
	glm::vec2 m_pos = glm::vec2(0);

	b2Fixture 	*m_fixture1 = nullptr, 
				*m_fixture2 = nullptr,
				*m_fixture3 = nullptr,
				*m_fixture4 = nullptr,
				*m_fixture5 = nullptr;
};

//-------------------------------------------------------------------
class Block_S 
{
public:
	virtual ~Block_S();
	
	virtual void init() = 0;
	//-----------------------------------
	virtual void createCounterpart(b2World *world, 
						std::vector<Block_S*> &blocks,
						const glm::vec2 &pos) = 0;
	//-----------------------------------
	virtual void update(
					std::vector< std::vector<Block_S*> > &TLBlocks,
				 	std::vector< std::vector<Block_S*> > &TRBlocks,
				 	std::vector< std::vector<Block_S*> > &BLBlocks,
				 	std::vector< std::vector<Block_S*> > &BRBlocks);

	// getters
	bool hasCounterpart() 		{ return m_hasCounterpart; }
	Block& getBlock() 			{ return m_block; }
	BLOCK_ID getBlockID()		{ return m_blockID; }

	// setters
	void setCounterpart(bool has) 		{ m_hasCounterpart = has; }
	void setBlock(Block &block) 		{ m_block = block; }
	void setBlockID(BLOCK_ID id) 		{ m_blockID = id; }
protected:
	bool m_hasCounterpart = false;			// 4 bytes
	Block m_block;							// ? bytes
	BLOCK_ID m_blockID = BLOCK_ID::NO_ID;	// 4 bytes
};


