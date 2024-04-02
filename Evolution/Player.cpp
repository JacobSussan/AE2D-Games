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
	AE::GLTexture texture = AE::ResourceManager::getTexture("Textures/blue_ninja.png");
	m_color = color;
	m_drawDims = drawDims;
	m_collisionDims = collisionDims;
	m_pos = pos;
	
	createBody(m_collisionDims, world);
	m_texture.init(texture, glm::ivec2(10, 2));		
}

//-------------------------------------------------------------------
void Player::createBody(const glm::vec2& collisionDims, b2World* world) 
{
	float density = 1.0f;
	float friction = 0.1f;
	bool fixedRotation = true;
    
	// Make the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(m_pos.x, m_pos.y);
    bodyDef.fixedRotation = fixedRotation;
    m_body = world->CreateBody(&bodyDef);

    // Create the box
    b2PolygonShape boxShape;
    boxShape.SetAsBox(collisionDims.x / 2.0f, (collisionDims.y - collisionDims.x) / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    m_fixtures[0] = m_body->CreateFixture(&fixtureDef);

    // Create the circle shape
    b2CircleShape circleShape;
    circleShape.m_radius = collisionDims.x / 2.0f;
    
    b2FixtureDef circleDef;
    circleDef.shape = &circleShape;
    circleDef.density = density;
    circleDef.friction = friction;

    // Bottom circle
    circleShape.m_p.Set(0.0f, (-collisionDims.y + collisionDims.x) / 2.0f);
    m_fixtures[1] = m_body->CreateFixture(&circleDef);

    // Top Circle
    circleShape.m_p.Set(0.0f, (collisionDims.y - collisionDims.x) / 3.0f);
    m_fixtures[1] = m_body->CreateFixture(&circleDef);
}

//-------------------------------------------------------------------
void Player::draw(AE::SpriteBatch& spriteBatch, AE::IMainGame* game)
{
	//animation
	int tileIndex, numTiles;
	float animationSpeed = 12 / game->getFps();
	glm::vec2 velocity;
	velocity.x = m_body->GetLinearVelocity().x;
	velocity.y = m_body->GetLinearVelocity().y;
	
	if (m_onGround) {
		if (m_punching) {
			numTiles = 4;
			tileIndex = 1;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animationTime = 0.0f;
			}
		} else if (abs(velocity.x) > 1.0f) { // running
			numTiles = 6;
			tileIndex = 10;
			animationSpeed *= abs(velocity.x) * 0.125f;
			if (m_moveState != PlayerMoveState::RUNNING) {
				m_animationTime = 0.0f;
			}
			m_moveState = PlayerMoveState::RUNNING;
		} else { // standing
			numTiles = 1;
			tileIndex = 0;
			m_moveState = PlayerMoveState::STANDING;
		}
	} else { // in the air
		if (m_punching) {
			numTiles = 1;
			tileIndex = 18;
			animationSpeed *= 0.25f;
			if (m_moveState != PlayerMoveState::PUNCHING) {
				m_moveState = PlayerMoveState::PUNCHING;
				m_animationTime = 0.0f;
			}
		} else if (abs(velocity.x) > 5.0f) {
			numTiles = 1;
			tileIndex = 10;
			m_moveState = PlayerMoveState::RUNNING_IN_AIR;
		} else if (velocity.y <= 0.0f) { // falling
			numTiles = 1;
			tileIndex = 17;
			m_moveState = PlayerMoveState::FALLING;
		} else { // jumping
			numTiles = 1;
			tileIndex = 16;
			m_moveState = PlayerMoveState::JUMPING;
		}	
	}

	// increment animation time
	m_animationTime += animationSpeed;
	
	// check if done punching
	if (m_animationTime > numTiles) {
		m_punching = false;
	}
	
	// apply animation
	tileIndex += (int)m_animationTime % numTiles;

	//get uv cords from tile index
	glm::vec4 uvRect = m_texture.getUVs(tileIndex);
	
	// check direction
	if (m_direction == -1) {
		uvRect.x += 1.0f / m_texture.dims.x;
		uvRect.z = -uvRect.z;
	}
	
	glm::vec4 destRect;
	destRect.x = m_body->GetPosition().x - m_drawDims.x / 2.0f;
	destRect.y = m_body->GetPosition().y - m_drawDims.y / 2.0f;
	destRect.z = m_drawDims.x;
	destRect.w = m_drawDims.y;
	//-----------------------------------
	spriteBatch.draw(destRect,
					 uvRect, 
					 m_texture.texture.id,
					 0.0f,
					 m_color,
					 m_body->GetAngle());
	//-----------------------------------
}

//-------------------------------------------------------------------
void Player::drawDebug(AE::DebugRenderer& debugRenderer)
{
	AE::Color_rgb8 color(255,255,255,255);

	b2CircleShape* circleShape;
	b2PolygonShape* polyShape;
	b2Vec2 worldPos;
	
	// loop fixtures
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext()) {
		b2Shape::Type shapeType = f->GetType();
		if (shapeType == b2Shape::e_circle) {
			circleShape = (b2CircleShape*)f->GetShape();
			worldPos = m_body->GetWorldPoint(circleShape->GetVertex(0));
			// draw debug circle
			debugRenderer.drawCircle(glm::vec2(worldPos.x, worldPos.y), color, circleShape->m_radius);
		} else if (shapeType == b2Shape::e_polygon) {

			polyShape = (b2PolygonShape*)f->GetShape();
			worldPos = m_body->GetWorldPoint(polyShape->GetVertex(0));

			glm::vec4 destRect;

			glm::vec2 shapeDims;
			shapeDims.x = polyShape->GetVertex(2).x - polyShape->GetVertex(0).x;
			shapeDims.y = polyShape->GetVertex(1).x - polyShape->GetVertex(3).y;
			
			destRect.x = m_body->GetPosition().x - shapeDims.x / 2.0f;
			destRect.y = m_body->GetPosition().y - (shapeDims.y - shapeDims.x) / 2.0f;
			destRect.z = shapeDims.x;
			destRect.w = shapeDims.y - shapeDims.x;

			// draw debug box
			debugRenderer.drawBox(destRect, color, m_body->GetAngle());
		} 
	}
}

//-------------------------------------------------------------------
bool Player::update(
			float 								 &tps,
			std::vector<Box> 					 &boxes,
			std::vector<chunk_S> 				 &chunks,
			b2World 			 				 *world		)
			//-----------------------------------
{
	bool rv = false;
	m_onGround = false;
	for (b2ContactEdge* ce = m_body->GetContactList(); ce != nullptr; ce = ce->next) {
		b2Contact* c = ce->contact;
		if (c->IsTouching()) {
			b2WorldManifold manifold;
			c->GetWorldManifold(&manifold);
			//-----------------------------------
			// check if the points are below
			for (int i = 0; i < b2_maxManifoldPoints; i++) {
				if (manifold.points[i].y < m_body->GetPosition().y 
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
			m_body->ApplyForceToCenter(b2Vec2(-(tps * 1.5f), 0.0), true);
			m_direction = -1;
			slowPlayer = false;
		} else if (m_pressedKeys[i] == SDLK_d) {
			m_body->ApplyForceToCenter(b2Vec2(tps * 1.5f, 0.0), true);
			m_direction = 1;
			slowPlayer = false;
		}
		//-----------------------------------
		// limit max speed the player can move
		float MAX_SPEED = 10.0f;
		if (m_body->GetLinearVelocity().x < -MAX_SPEED) {
			m_body->SetLinearVelocity(b2Vec2(-MAX_SPEED, m_body->GetLinearVelocity().y));
		} else if (m_body->GetLinearVelocity().x > MAX_SPEED) {
			m_body->SetLinearVelocity(b2Vec2(MAX_SPEED, m_body->GetLinearVelocity().y));
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
				m_body->ApplyLinearImpulse(b2Vec2(0.0f, 50.0f), b2Vec2(0.0f, 0.0f), true);
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
				Block* newBlock = Helper::getBlockTypeFromID(m_selectedBlock);
				
				// loop chunks
				for (unsigned long i = 0; i < chunks.size(); i++) {
				// if chunk.m_pos == mousePos / 16 then this is the right chunk
					if (chunks[i].m_pos == glm::vec2(mousePos.x / 16, mousePos.y / 16)) {	
						// get block in chunk by doing mousepos % 16
						// delete the old block
						(*chunks[i].m_chunk->getBlocks())[(int)mousePos.y % 16][(int)mousePos.x % 16]->destroy();
						// init new block
						newBlock->init();
						// set the spot to the new block
						chunks[i].m_chunk->setBlock(newBlock, (int)mousePos.x % 16, (int)mousePos.y % 16);
						rv = true;
					}
				}
			} 
		}
	
		// remove block at mouse position if one is there
		if (m_pressedKeys[i] == SDL_BUTTON_LEFT) {
			// get and store mouse pos
			glm::vec2 mousePos = m_worldMouseCoords;
			mousePos.x = round(mousePos.x);
			mousePos.y = round(mousePos.y);
			// get block at mouse pos
			Block *currentBlock = Helper::getBlock(mousePos, chunks);
			// check if block is not air
			if (currentBlock->getBlockID() != BLOCK_ID::AIR) {
				// loop chunks
				for (unsigned long j = 0; j < chunks.size(); j++) {
					std::vector<std::vector<Block* > > *blocks = chunks[j].m_chunk->getBlocks();
					// get correct chunks
					if (glm::vec2(floor(mousePos.x / 16.0f), floor(mousePos.y / 16.0f)) == chunks[j].m_pos) {
						// loop blocks
						for (unsigned long k = 0; k < blocks->size(); k++) {
							for (unsigned long l = 0; l < blocks[i].size(); l++) {
								if ((*blocks)[k][l] == currentBlock) {
									(*blocks)[k][l]->destroy();
								}
							}
						}
					}
				}
				// set block id to 1 (air)
				currentBlock->setBlockID(BLOCK_ID::AIR);
				// destory body
				currentBlock->destroy();
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
		m_body->SetLinearVelocity(b2Vec2(
						  m_body->GetLinearVelocity().x * 0.95 * (tps / 60.0f), 
						  m_body->GetLinearVelocity().y));	
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

