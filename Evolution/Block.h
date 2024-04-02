#pragma once

#include "AE2D/Vertex.h"
#include "AE2D/SpriteBatch.h"
#include "AE2D/DebugRenderer.h"
#include "AE2D/GLTexture.h"
#include "BlockData.h"

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <random>

enum struct BLOCK_QUAD : unsigned long { TL, TR, BL, BR, NO_QUAD };

class Block_S;
class Brick_S;

//-------------------------------------------------------------------
class Block
{
public:
	virtual void init() = 0;
	//-----------------------------------
	void createBody(
		b2World* world, 
		const glm::vec2& pos, 
		bool fixedRotation,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//-----------------------------------
	void destroy();
	void draw(AE::SpriteBatch& spriteBatch);
	void drawDebug(AE::DebugRenderer &debugRenderer);
	//-----------------------------------
	b2Body* 		getBody()       { return m_body; }
	glm::vec2&		getDimentions() { return m_dimensions; }
	AE::Color_rgb8& getColor() 		{ return m_color; }
	glm::vec2 		getPos() 		{ return m_pos; }	
	
	
	// getters
	BLOCK_ID getBlockID()		{ return m_blockID; }

	// setters
	void setBlockID(BLOCK_ID id) 		{ m_blockID = id; }
//-------------------------------------------------------------------
protected:
	b2Body *m_body = nullptr;
	glm::vec2 m_pos = glm::vec2(0);
	b2Fixture 	*m_fixture1 = nullptr, 
				*m_fixture2 = nullptr,
				*m_fixture3 = nullptr,
				*m_fixture4 = nullptr,
				*m_fixture5 = nullptr;
	glm::vec4 m_uvRect;
	AE::GLTexture m_texture;
	AE::Color_rgb8 m_color;
	glm::vec2 m_dimensions;
	BLOCK_ID m_blockID;				// 4 bytes
};

