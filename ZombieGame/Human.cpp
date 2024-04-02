#include "Human.h"
#include "AE2D/ResourceManager.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

//-------------------------------------------------------------------
Human::Human() : m_frames(0) 
{

}

//-------------------------------------------------------------------
Human::~Human() 
{

}

//-------------------------------------------------------------------
void Human::init(float speed, glm::vec2 pos) 
{
	static std::mt19937 rand(time(nullptr));
	static std::uniform_real_distribution<float> randDirection(-1.0f, 1.0f);

	m_health = 20.0f;

	m_color = AE::Color_rgb8(255, 255, 255, 255);

	m_speed = speed;
	m_position = pos;
	m_direction = glm::vec2(randDirection(rand), randDirection(rand));
	m_textureID = AE::ResourceManager::getTexture("Textures/human.png").id;

	if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);
}

//-------------------------------------------------------------------
void Human::update(const std::vector<std::string>& levelData,
				   const std::vector<Human*>& humans,
				   const std::vector<Zombie*>& zombies,
				   float deltaTime) 
{
	static std::mt19937 rand(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-20.0f, 20.0f);

	m_position += m_direction * m_speed * deltaTime;

	if (m_frames == 300) {
		m_direction = glm::rotate(m_direction, randRotate(rand));
		m_frames = 0;
	} else { m_frames++; }

	if (collideWithLevel(levelData)) { m_direction = glm::rotate(m_direction, randRotate(rand)); }
}

