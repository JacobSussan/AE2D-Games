#include "Bullet.h"
#include "AE2D/ResourceManager.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"

//-------------------------------------------------------------------
Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float damage) {
	m_position = pos;
	_direction = dir;
	_speed = speed;
	_damage = damage;
}

//-------------------------------------------------------------------
Bullet::~Bullet() 
{

}
//-------------------------------------------------------------------
bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime) 
{
	m_position += _direction * _speed * deltaTime;
	return collideWithWorld(levelData);
}

//-------------------------------------------------------------------
void Bullet::draw(AE::SpriteBatch& spriteBatch) 
{
	glm::vec4 destRect(m_position.x + BULLED_RADIUS,
					   m_position.y + BULLED_RADIUS,
					   BULLED_RADIUS * 2,
					   BULLED_RADIUS * 2);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	AE::Color_rgb8 Color_rgb8(75, 75, 75, 255);
	spriteBatch.draw(destRect, uvRect, AE::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, Color_rgb8);
}

//-------------------------------------------------------------------
bool Bullet::collideWithAgent(Agent* agent) 
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLED_RADIUS;

	glm::vec2 centerPosA = m_position;
	glm::vec2 centerPosB = agent->getPos() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0) return true;
	return false;
}

//-------------------------------------------------------------------
bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) 
{
	glm::ivec2 gridPos;
	gridPos.x = floor(m_position.x / TILE_WIDTH);
	gridPos.y = floor(m_position.y / TILE_WIDTH);

	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return true;
	}

	return (levelData[gridPos.y][gridPos.x] != '.');
}

