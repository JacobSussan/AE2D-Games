#include "Player.h"	
#include "AE2D/ResourceManager.h"
#include "Helper.h"
#include <SDL/SDL.h>

#include <iostream>

Player::Player (){ }
//-------------------------------------------------------------------
Player::~Player (){ }
//-------------------------------------------------------------------
void Player::init(b2World* world, 
				const glm::vec2& pos,
				const glm::vec2& drawDims,
				const glm::vec2& collisionDims, 
				const AE::Color_rgb8 color)
				//-----------------------------------
{
	m_textureFilePath = "Textures/blue_ninja.png";
	AE::GLTexture texture = AE::ResourceManager::getTexture(m_textureFilePath);
	m_color = color;
	m_drawDims = drawDims;
	m_collisionDims = collisionDims;
	m_pos = pos;

	createCapsule(m_collisionDims, world);
	m_texture.init(texture, glm::ivec2(10, 2));
}

//-------------------------------------------------------------------
void Player::createCapsule(const glm::vec2& collisionDims, b2World* world) 
{
	float density = 1.0f;
	float friction = 0.1f;
	bool fixedRotation = true;
	
	std::vector<b2Body*> bodies;
	//-----------------------------------
	// Make body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_pos.x, m_pos.y);
	bodyDef.fixedRotation = fixedRotation;
	bodies.resize(12);
	//-----------------------------------
	// Create the circles
	b2CircleShape circleShape;
	b2FixtureDef circleDef;
	circleDef.shape = &circleShape;
	circleDef.density = density;
	circleDef.friction = friction;

	// create the body for each animation frame
	
	//-----------------------------------
	/// Standing facing right 
	bodies[0] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 4.0f;
	circleShape.m_p.Set(0.05f, (collisionDims.y - collisionDims.x) / 2.3f);
	bodies[0]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(0.4f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[0]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(0.1f, (collisionDims.y - collisionDims.x) / 1.15f);
	bodies[0]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;	
	circleShape.m_p.Set(0.05f, 0.0f);					
	bodies[0]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.2f, (-collisionDims.y + collisionDims.x) / 2.0f);
	bodies[0]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;
	circleShape.m_p.Set(-0.3f, (-collisionDims.y + collisionDims.x));					
	bodies[0]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;
	circleShape.m_p.Set(0.3f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[0]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;
	circleShape.m_p.Set(0.4f, (-collisionDims.y + collisionDims.x));					
	bodies[0]->CreateFixture(&circleDef);

	//-----------------------------------
	/// Standing facing left
	bodies[1] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 4.0f;
	circleShape.m_p.Set(-0.05f, (collisionDims.y - collisionDims.x) / 2.3f);
	bodies[1]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(-0.4f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[1]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(-0.1f, (collisionDims.y - collisionDims.x) / 1.15f);
	bodies[1]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(-0.05f, 0.0f);					
	bodies[1]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.2f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[1]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.3f, (-collisionDims.y + collisionDims.x));					
	bodies[1]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(-0.3f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[1]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.4f, (-collisionDims.y + collisionDims.x));					
	bodies[1]->CreateFixture(&circleDef);
	
	//-----------------------------------
	/// Running right 
	bodies[2] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 6.0f;
	circleShape.m_p.Set(0.2f, (collisionDims.y - collisionDims.x) / 4.5f);
	bodies[2]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(-0.125f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[2]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(0.50f, (collisionDims.y - collisionDims.x) / 1.65f);
	bodies[2]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(0.05f, -0.2f);					
	bodies[2]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.2f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[2]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.3f, (-collisionDims.y + collisionDims.x));					
	bodies[2]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(0.3f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[2]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.4f, (-collisionDims.y + collisionDims.x));					
	bodies[2]->CreateFixture(&circleDef);
	
	//-----------------------------------
	/// Running left 
	bodies[3] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 6.0f;
	circleShape.m_p.Set(-0.2f, (collisionDims.y - collisionDims.x) / 4.5f);
	bodies[3]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(0.125f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[3]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(-0.50f, (collisionDims.y - collisionDims.x) / 1.65f);
	bodies[3]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(-0.05f, -0.2f);					
	bodies[3]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.2f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[3]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.3f, (-collisionDims.y + collisionDims.x));					
	bodies[3]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(-0.3f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[3]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.4f, (-collisionDims.y + collisionDims.x));					
	bodies[3]->CreateFixture(&circleDef);
	
	//-----------------------------------
	/// Running right in air 
	bodies[4] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 6.0f;
	circleShape.m_p.Set(0.2f, (collisionDims.y - collisionDims.x) / 4.5f);
	bodies[4]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(-0.125f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[4]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(0.50f, (collisionDims.y - collisionDims.x) / 1.65f);
	bodies[4]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(0.05f, -0.2f);					
	bodies[4]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;		
	circleShape.m_p.Set(-0.125f, (-collisionDims.y + collisionDims.x) / 1.5f);					
	bodies[4]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.4f, (-collisionDims.y + collisionDims.x));					
	bodies[4]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(0.3f, (-collisionDims.y + collisionDims.x) / 2.4f);					
	bodies[4]->CreateFixture(&circleDef);

	//-----------------------------------
	/// Running left in air 
	bodies[5] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 6.0f;
	circleShape.m_p.Set(-0.2f, (collisionDims.y - collisionDims.x) / 4.5f);
	bodies[5]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(0.125f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[5]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(-0.50f, (collisionDims.y - collisionDims.x) / 1.65f);
	bodies[5]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(-0.05f, -0.2f);					
	bodies[5]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.125f, (-collisionDims.y + collisionDims.x) / 1.5f);					
	bodies[5]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.4f, (-collisionDims.y + collisionDims.x));					
	bodies[5]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(-0.3f, (-collisionDims.y + collisionDims.x) / 2.4f);					
	bodies[5]->CreateFixture(&circleDef);

	//-----------------------------------
	// jumping left 
	bodies[6] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;
	circleShape.m_p.Set(-0.05f, (collisionDims.y - collisionDims.x) / 2.3f);
	bodies[6]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(-0.3f, (collisionDims.y - collisionDims.x) / 12.0f);
	bodies[6]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 5.4f;
	circleShape.m_p.Set(-0.1f, (collisionDims.y - collisionDims.x) / 1.15f);
	bodies[6]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(-0.05f, 0.0f);					
	bodies[6]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.1f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[6]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.15f, (-collisionDims.y + collisionDims.x));					
	bodies[6]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 7.5f;			
	circleShape.m_p.Set(-0.2f, (-collisionDims.y + collisionDims.x) / 2.5f);					
	bodies[6]->CreateFixture(&circleDef);
		
	//-----------------------------------
	// jumping right 
	bodies[7] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;
	circleShape.m_p.Set(0.05f, (collisionDims.y - collisionDims.x) / 2.3f);
	bodies[7]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(0.3f, (collisionDims.y - collisionDims.x) / 12.0f);
	bodies[7]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 5.4f;
	circleShape.m_p.Set(0.1f, (collisionDims.y - collisionDims.x) / 1.15f);
	bodies[7]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(0.05f, 0.0f);					
	bodies[7]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.1f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[7]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.15f, (-collisionDims.y + collisionDims.x));					
	bodies[7]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 7.5f;			
	circleShape.m_p.Set(0.2f, (-collisionDims.y + collisionDims.x) / 2.5f);					
	bodies[7]->CreateFixture(&circleDef);
	
	//-----------------------------------
	// falling left
	bodies[8] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;
	circleShape.m_p.Set(-0.05f, (collisionDims.y - collisionDims.x) / 2.6f);
	bodies[8]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(0.4f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[8]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;
	circleShape.m_p.Set(-0.2f, (collisionDims.y - collisionDims.x) / 1.5f);
	bodies[8]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(0.0f, 0.0f);					
	bodies[8]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 7.0f;			
	circleShape.m_p.Set(0.1f, (-collisionDims.y + collisionDims.x) / 1.5f);					
	bodies[8]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(-0.075f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[8]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.1f, (-collisionDims.y + collisionDims.x));					
	bodies[8]->CreateFixture(&circleDef);	

	//-----------------------------------
	// falling right
	bodies[9] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;
	circleShape.m_p.Set(0.05f, (collisionDims.y - collisionDims.x) / 2.6f);
	bodies[9]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(-0.4f, (collisionDims.y - collisionDims.x) / 4.0f);
	bodies[9]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 5.0f;
	circleShape.m_p.Set(0.2f, (collisionDims.y - collisionDims.x) / 1.5f);
	bodies[9]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.0f;			
	circleShape.m_p.Set(-0.0f, 0.0f);					
	bodies[9]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 7.0f;			
	circleShape.m_p.Set(-0.1f, (-collisionDims.y + collisionDims.x) / 1.5f);					
	bodies[9]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 5.5f;			
	circleShape.m_p.Set(0.075f, (-collisionDims.y + collisionDims.x) / 2.0f);					
	bodies[9]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.1f, (-collisionDims.y + collisionDims.x));					
	bodies[9]->CreateFixture(&circleDef);	
	
	//-----------------------------------
	/// Kicking right 
	bodies[10] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 4.0f;
	circleShape.m_p.Set(0.05f, (collisionDims.y - collisionDims.x) / 3.75f);
	bodies[10]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(-0.6f, (collisionDims.y - collisionDims.x) / 3.0f);
	bodies[10]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(0.1f, (collisionDims.y - collisionDims.x) / 1.4f);
	bodies[10]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.3f, (-collisionDims.y + collisionDims.x) / 5.0f);					
	bodies[10]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(-0.7f, (-collisionDims.y + collisionDims.x) / 3.0f);					
	bodies[10]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 7.0f;			
	circleShape.m_p.Set(0.4f, 0.1f);					
	bodies[10]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 8.0f;			
	circleShape.m_p.Set(0.6f, 0.05f);					
	bodies[10]->CreateFixture(&circleDef);
	
	//-----------------------------------
	/// Kicking right 
	bodies[11] = world->CreateBody(&bodyDef);
	
	circleShape.m_radius = collisionDims.x / 4.0f;
	circleShape.m_p.Set(-0.05f, (collisionDims.y - collisionDims.x) / 3.75f);
	bodies[11]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 10.0f;
	circleShape.m_p.Set(0.6f, (collisionDims.y - collisionDims.x) / 3.0f);
	bodies[11]->CreateFixture(&circleDef);
	
	circleShape.m_radius = collisionDims.x / 4.4f;
	circleShape.m_p.Set(-0.1f, (collisionDims.y - collisionDims.x) / 1.4f);
	bodies[11]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.3f, (-collisionDims.y + collisionDims.x) / 5.0f);					
	bodies[11]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 6.0f;			
	circleShape.m_p.Set(0.7f, (-collisionDims.y + collisionDims.x) / 3.0f);					
	bodies[11]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 7.0f;			
	circleShape.m_p.Set(-0.4f, 0.1f);					
	bodies[11]->CreateFixture(&circleDef);

	circleShape.m_radius = collisionDims.x / 8.0f;			
	circleShape.m_p.Set(-0.6f, 0.05f);					
	bodies[11]->CreateFixture(&circleDef);

	//-----------------------------------
	// inti capsule
	m_capsule.init(bodies);
	m_capsule.setActiveBody(m_currentBodyIndex, 0);
}

//-------------------------------------------------------------------
bool Player::update(
			float 								 &tps,
			std::vector<Box> 					 &boxes,
			std::vector<Block_S*> 				 &blocks,
			std::vector< std::vector<Block_S*> > &TLBlocks,
			std::vector< std::vector<Block_S*> > &TRBlocks,
			std::vector< std::vector<Block_S*> > &BLBlocks,
			std::vector< std::vector<Block_S*> > &BRBlocks,
			b2World 				 			 *world		)
			//-----------------------------------
{
	bool rv = false;
	// check if enough time has passed that we can update the current body
	if (m_framesSinceLastBodyUpdate > tps / 6.0f) {					// check if we need to update the body
		if (m_moveState == PlayerMoveState::STANDING 				// standing facing right
						&& m_direction == 1 && m_currentBodyIndex != 0) {	
			m_capsule.setActiveBody(m_currentBodyIndex, 0);
		} else if (m_moveState == PlayerMoveState::STANDING 		// standing facing left
						&& m_direction == -1 && m_currentBodyIndex != 1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 1);
		} else if (m_moveState == PlayerMoveState::RUNNING 			// running facing right
						&& m_direction == 1 && m_currentBodyIndex != 2){
			m_capsule.setActiveBody(m_currentBodyIndex, 2);
		} else if (m_moveState == PlayerMoveState::RUNNING 			// running facing left
						&& m_direction == -1 && m_currentBodyIndex != 3) {
			m_capsule.setActiveBody(m_currentBodyIndex, 3);
		} else if (m_moveState == PlayerMoveState::RUNNING_IN_AIR 	// running in air facing right
						&& m_direction == 1 && m_currentBodyIndex != 4 && m_onGround == false) {
			m_capsule.setActiveBody(m_currentBodyIndex, 4);
		} else if (m_moveState == PlayerMoveState::RUNNING_IN_AIR 	// running in air facing left
						&& m_direction == -1 && m_currentBodyIndex != 5 && m_onGround == false) {
			m_capsule.setActiveBody(m_currentBodyIndex, 5);
		} else if (m_moveState == PlayerMoveState::JUMPING 			// jumping facing left
						&& m_currentBodyIndex != 6 && m_onGround == false && m_direction == -1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 6);
		} else if (m_moveState == PlayerMoveState::JUMPING 			// jumping facing right
						&& m_currentBodyIndex != 7 && m_onGround == false && m_direction == 1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 7);
		} else if (m_moveState == PlayerMoveState::FALLING 			// falling facing left
						&& m_currentBodyIndex != 8 && m_onGround == false && m_direction == -1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 8);
		} else if (m_moveState == PlayerMoveState::FALLING 			// falling facing right
						&& m_currentBodyIndex != 9 && m_onGround == false && m_direction == 1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 9);
		} else if (m_moveState == PlayerMoveState::PUNCHING 		// kicking in air facing right
						&& m_currentBodyIndex != 10 && m_onGround == false && m_direction == 1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 10);
		} else if (m_moveState == PlayerMoveState::PUNCHING 		// kicking in air facing left
						&& m_currentBodyIndex != 11 && m_onGround == false && m_direction == -1) {
			m_capsule.setActiveBody(m_currentBodyIndex, 11);
		//-----------------------------------
		} else if (m_moveState == PlayerMoveState::PUNCHING 		// punching on the ground facing right
						&& m_direction == 1 && m_currentBodyIndex != 0 && m_onGround == true) {
			m_capsule.setActiveBody(m_currentBodyIndex, 0);
		} else if (m_moveState == PlayerMoveState::PUNCHING 		// punching on the ground facing left
						&& m_direction == -1 && m_currentBodyIndex != 1 && m_onGround == true) {
			m_capsule.setActiveBody(m_currentBodyIndex, 1);
		} 
	// if we did update the body, set m_famesSinceLastBodyUpdate to 0 again
	m_framesSinceLastBodyUpdate = 0;
	} else {
		// if not enough frames have passed to update the body, add one frame to the counter.
		m_framesSinceLastBodyUpdate++;
	}

	m_onGround = false;
	for (b2ContactEdge* ce = m_capsule.getBody(m_currentBodyIndex)->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			//-----------------------------------
			// check if the points are below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_capsule.getBody(m_currentBodyIndex)->GetPosition().y 
								- m_collisionDims.y / 2.0f + (m_collisionDims.x / 2.0f) + 0.01f) {
				m_onGround = true;
				break;
				}
			}
		}
	}
	bool slowPlayer = true;
	for (unsigned long i = 0; i < m_pressedKeys.size(); i++) {	
		//-----------------------------------
		// Check for left/right movement 
		if (m_pressedKeys[i] == SDLK_a) {
			m_capsule.getBody(m_currentBodyIndex)->ApplyForceToCenter(b2Vec2(-(tps * 1.5f), 0.0), true);
			m_direction = -1;
			slowPlayer = false;
		} else if (m_pressedKeys[i] == SDLK_d) {
			m_capsule.getBody(m_currentBodyIndex)->ApplyForceToCenter(b2Vec2(tps * 1.5f, 0.0), true);
			m_direction = 1;
			slowPlayer = false;
		}
		//-----------------------------------
		// limit max speed the player can move
		float MAX_SPEED = 10.0f;
		if (m_capsule.getBody(m_currentBodyIndex)->GetLinearVelocity().x < -MAX_SPEED) {
			m_capsule.getBody(m_currentBodyIndex)->SetLinearVelocity(b2Vec2(-MAX_SPEED, 
									m_capsule.getBody(m_currentBodyIndex)->GetLinearVelocity().y));
		} else if (m_capsule.getBody(m_currentBodyIndex)->GetLinearVelocity().x > MAX_SPEED) {
			m_capsule.getBody(m_currentBodyIndex)->SetLinearVelocity(b2Vec2(MAX_SPEED, 
									m_capsule.getBody(m_currentBodyIndex)->GetLinearVelocity().y));
		}
		//-----------------------------------
		// check for punch
		if (m_pressedKeys[i] == SDL_BUTTON_LEFT) {
			m_punching = true;
		}

		bool wasKeyDown = false;

		// jumping 
		if (m_pressedKeys[i] == SDLK_SPACE) {
			for (unsigned int i = 0; i < m_prevPressedKeys.size(); i++) {
				if (m_prevPressedKeys[i] == SDLK_SPACE) {
					wasKeyDown = true;
				}
			}
			if (m_onGround && !wasKeyDown) {
				m_capsule.getBody(m_currentBodyIndex)->ApplyLinearImpulse(b2Vec2(0.0f, 20.0f), b2Vec2(0.0f, 0.0f), true);
			}
		}
		wasKeyDown = false;
		// create new box at mouse position
		if (m_pressedKeys[i] == SDL_BUTTON_MIDDLE) {
			for (unsigned int i = 0; i < m_prevPressedKeys.size(); i++) {
				if (m_prevPressedKeys[i] == SDL_BUTTON_MIDDLE) {
					wasKeyDown = true;
				}
			}
			
			if (!wasKeyDown) {
				std::uniform_real_distribution<float> size(0.5, 2.5);
				std::uniform_int_distribution<int> color(0, 255);
				Box newBox;
				//-----------------------------------
				AE::Color_rgb8 randColor(color(randomGenerator), 
										 color(randomGenerator), 
										 color(randomGenerator), 255);
				AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/red_bricks.png");
				//-----------------------------------
				newBox.init(world, m_worldMouseCoords, glm::vec2(size(randomGenerator), 
										size(randomGenerator)), texture, randColor, false, true);
				//-----------------------------------
				boxes.push_back(newBox);
			}
		}
	
		// create new block at mouse postion
		if (m_pressedKeys[i] == SDL_BUTTON_RIGHT) {
			// get and store mouse pos		
			glm::vec2 mousePos = m_worldMouseCoords;
			mousePos.x = round(mousePos.x);
			mousePos.y = round(mousePos.y);

			// check if there is anything in the way
			if (!Helper::checkBox2DPos(world, mousePos)) {
				// create pointer to new block
				Block_S* blockToCreate = Helper::getBlockTypeFromID(m_selectedBlock);
				
				// make block_s
				// get the quad and set newBlock_S to the correct position in the correct quad
				BLOCK_QUAD q = Helper::getBlock_SQuad(mousePos);
				if (q == BLOCK_QUAD::TR) {
					delete TRBlocks[mousePos.y][mousePos.x];				// delete the air block
					TRBlocks[mousePos.y][mousePos.x] = blockToCreate;		// set this spot to block with id of m_selectedBlock
					TRBlocks[mousePos.y][mousePos.x]->init();				// init the new block
				} else if (q == BLOCK_QUAD::TL) {
					delete TLBlocks[mousePos.y][-mousePos.x];
					TLBlocks[mousePos.y][-mousePos.x] = blockToCreate;
					TLBlocks[mousePos.y][-mousePos.x]->init();
				} else if (q == BLOCK_QUAD::BR) {
					delete BRBlocks[-mousePos.y][mousePos.x];
					BRBlocks[-mousePos.y][mousePos.x] = blockToCreate;
					BRBlocks[-mousePos.y][mousePos.x]->init();
				} else if (q == BLOCK_QUAD::BL) {
					delete BLBlocks[-mousePos.y][-mousePos.x];
					BLBlocks[-mousePos.y][-mousePos.x] = blockToCreate;
					BLBlocks[-mousePos.y][-mousePos.x]->init();
				}
				rv = true;
			} 
		}
	
		// remove block at mouse position if one is there
		if (m_pressedKeys[i] == SDL_BUTTON_LEFT) {
			// get and store mouse pos
			glm::vec2 mousePos = m_worldMouseCoords;
			mousePos.x = round(mousePos.x);
			mousePos.y = round(mousePos.y);
			// get block at mouse pos	
			Block_S *currentBlock_S = Helper::getBlock_SFromPos(mousePos, TLBlocks, TRBlocks, BLBlocks, BRBlocks);
			// check if block is not air
			if (currentBlock_S->getBlockID() != BLOCK_ID::AIR && currentBlock_S->hasCounterpart()) {
				// loop the vector of active blocks and remove this one
				for (unsigned long i = 0; i < blocks.size(); i++) {
					if (blocks[i] == currentBlock_S) {
						blocks.erase(blocks.begin() + i);
					}
				}
				// set block id to 1 (air)
				currentBlock_S->setBlockID(BLOCK_ID::AIR);
				// destory body
				currentBlock_S->getBlock().destroy(TLBlocks, TRBlocks, BLBlocks, BRBlocks);
				rv = true;
			}
		}
	
		// block selection
		if (m_pressedKeys[i] == SDLK_1) {
			m_selectedBlock = BLOCK_ID::BRICK;
		} else if (m_pressedKeys[i] == SDLK_2) {
			m_selectedBlock = BLOCK_ID::RAINBOW_BRICK;
		} else if (m_pressedKeys[i] == SDLK_3) {
			m_selectedBlock = BLOCK_ID::LIGHT_BRICK;
		} else if (m_pressedKeys[i] == SDLK_4) {
			m_selectedBlock = BLOCK_ID::GLASS;
		}
	}

	if (slowPlayer) {
		m_capsule.getBody(m_currentBodyIndex)->SetLinearVelocity(b2Vec2(
						  m_capsule.getBody(m_currentBodyIndex)->GetLinearVelocity().x * 0.95 * (tps / 60.0f), 
						  m_capsule.getBody(m_currentBodyIndex)->GetLinearVelocity().y));	
	}

	glm::vec2 tmpPlayerPos(0,0);
	if (abs(tmpPlayerPos.x) + 1 < abs(getPosition().x) 
	 || abs(tmpPlayerPos.y) + 1 < abs(getPosition().y)
	 || abs(tmpPlayerPos.x) - 1 > abs(getPosition().x) 
	 || abs(tmpPlayerPos.y) - 1 > abs(getPosition().y) ) {
		rv = true;
		tmpPlayerPos = getPosition();
	}

	m_prevPressedKeys = m_pressedKeys;
	return rv;
}

