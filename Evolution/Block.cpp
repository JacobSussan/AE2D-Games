#include "Block.h"

#include "Helper.h"
#include "AE2D/AEErrors.h"
#include "AE2D/ResourceManager.h"

#include <iostream>

// Block_S = a simple block def for storage
// Block   = the actual Box2D block, only created when a player is near enough to see it.

//-------------------------------------------------------------------
void Block::createBody(
				b2World* world,
				const glm::vec2& pos,
				bool fixedRotation,
				glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) */)
				//-----------------------------------
{
	if (m_blockID == BLOCK_ID::NO_ID || m_blockID == BLOCK_ID::AIR) {
		return;
	}

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

	//TODO: we should only need 4 edge shapes to make a square...
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
	// line in center (for detection, fix)
	edgeShape5.Set(b2Vec2(0.1, 0.1), b2Vec2(-0.1, -0.1));

	m_fixture1 = m_body->CreateFixture(&edgeShape1, 0);
	m_fixture2 = m_body->CreateFixture(&edgeShape2, 0);
	m_fixture3 = m_body->CreateFixture(&edgeShape3, 0);
	m_fixture4 = m_body->CreateFixture(&edgeShape4, 0);
	m_fixture5 = m_body->CreateFixture(&edgeShape5, 0);
}

//-------------------------------------------------------------------
void Block::destroy()
{
	// destroy m_body and set it to nullptr
	m_body->GetWorld()->DestroyBody(m_body);
	m_body = nullptr;
	
	// set hasCounterpart of the corresponding Block_S to false and
	// set Block_S m_block pointer to nullptr	
}

//-------------------------------------------------------------------
void Block::draw(AE::SpriteBatch& spriteBatch)
{
	if (m_blockID == BLOCK_ID::AIR || m_blockID == BLOCK_ID::NO_ID) {
		return;
	}

	std::cout << (int)m_blockID << std::endl;

	glm::vec4 destRect;
	destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
	destRect.z = m_dimensions.x;
	destRect.w = m_dimensions.y;
	//-----------------------------------
	spriteBatch.draw(destRect,
					 m_uvRect,
					 m_texture.id,
					 0.0f,
					 m_color,
					 m_body->GetAngle());
	//-----------------------------------
}

//-------------------------------------------------------------------
void Block::drawDebug(AE::DebugRenderer &debugRenderer)
{			
	glm::vec4 destRect;

	destRect.x = m_body->GetPosition().x - getDimentions().x / 2.0f;
	destRect.y = m_body->GetPosition().y - getDimentions().y / 2.0f;
	destRect.z = getDimentions().x;
	destRect.w = getDimentions().y;

	debugRenderer.drawBox(destRect, AE::Color_rgb8(255, 255, 255, 255), m_body->GetAngle());
}

