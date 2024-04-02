#include "Zombie.h"
#include "Human.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include "AE2D/ResourceManager.h"

Zombie::Zombie() : m_frames(0) 
{

}


Zombie::~Zombie() 
{

}

void Zombie::init(float speed, float health, glm::vec2 pos, GLuint textureID) 
{		
	static std::mt19937 rand(time(nullptr));
	static std::uniform_real_distribution<float> randDirection(-1.0f, 1.0f);

	m_health = health;
	m_speed = speed;
	m_position = pos;
	m_color = AE::Color_rgb8(255, 255, 255, 255);
	m_textureID = textureID;

	m_direction = glm::vec2(randDirection(rand), randDirection(rand));

	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);
}

void Zombie::update(const std::vector<std::string>& levelData,
					const std::vector<Human*>& humans,
					const std::vector<Zombie*>& zombies,
					float deltaTime) 
{
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPos() - m_position);
		m_position += m_direction * m_speed * deltaTime;
	} else {
		static std::mt19937 rand(time(nullptr));
		std::uniform_real_distribution<float> randRotate(-20.0f, 20.0f);

		m_position += m_direction * m_speed * deltaTime;

		if (m_frames == 300) {
			m_direction = glm::rotate(m_direction, randRotate(rand));
			m_frames = 0;
		} else { m_frames++; }

		if (collideWithLevel(levelData)) { m_direction = glm::rotate(m_direction, randRotate(rand)); }
		return;
	}
	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(const std::vector<Human*>& humans) 
{
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;

	for (unsigned int i = 0; i < humans.size(); i++) 
	{
		// Get the direction vector
		glm::vec2 distVec = humans[i]->getPos() - m_position;
		// Get distance
		float distance = glm::length(distVec);

		// If this person is closer than the closest person, this is the new closest
		if (distance < smallestDistance)
	   	{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	if (smallestDistance < 1500) return closestHuman;

	return nullptr;
}

