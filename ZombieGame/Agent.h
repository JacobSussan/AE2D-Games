#pragma once

#include <glm/glm.hpp>
#include "AE2D/SpriteBatch.h"

#include <vector>
#include <string>
//-------------------------------------------------------------------
const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

//-------------------------------------------------------------------
class Agent 
{
public:
	Agent();
	virtual ~Agent();
	virtual void update(const std::vector<std::string>& levelData,
						const std::vector<Human*>& humans,
						const std::vector<Zombie*>& zombies, 
						float deltaTime) = 0;
	void draw(AE::SpriteBatch& _spriteBatch);
	bool damage(float damage);
	bool collideWithLevel(const std::vector<std::string>& levelData);
	bool collideWithAgent(Agent* agent);

	//get
	glm::vec2 getPos() const { return m_position; }

protected:
	void checkTilePos(std::vector<glm::vec2>& collideTilePositions, 
		const std::vector<std::string>& levelData, float x, float y);
	
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 m_position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0f);
	AE::Color_rgb8 m_color;
	float m_speed;
	float m_health;
	GLuint m_textureID;
};

