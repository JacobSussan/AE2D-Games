#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "AE2D/SpriteBatch.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"

class Human;
class Zombie;
class Agent;

const int BULLED_RADIUS = 5;

//-------------------------------------------------------------------
class Bullet 
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float damage);
	~Bullet();

	bool update(const std::vector<std::string>& levelData, float deltaTime);
	void draw(AE::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* agent);

	//get
	float getDamage() const { return _damage; }
	glm::vec2 getPos() const { return m_position; }

private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	int _lifeTime;
	float _damage;
	float _speed;
	glm::vec2 _direction, m_position;
};

