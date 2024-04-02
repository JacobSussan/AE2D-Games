#include "Gun.h"

#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

//-------------------------------------------------------------------
Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, AE::SoundEffect fireSound) :
	m_name(name),
	m_fireRate(fireRate),
	m_bulletsPerShot(bulletsPerShot),
	m_spread(spread),
	m_bulletDamage(bulletDamage),
	m_bulletSpeed(bulletSpeed),
	m_frameCounter(0),
	m_fireSound(fireSound) 
{

}

//-------------------------------------------------------------------
Gun::~Gun()
{

}

//-------------------------------------------------------------------
void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) 
{
	m_frameCounter+= 1.0f * deltaTime;
	// After a certain number of frames has passed we fire our gun
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		fire(direction, position, bullets);
		m_frameCounter = 0;
	}
}

//-------------------------------------------------------------------
void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets)
{
	static std::mt19937 rand(time(nullptr));
	std::uniform_real_distribution<float> randSpread(-m_spread, m_spread);
	m_fireSound.play();

	for (int i = 0; i < m_bulletsPerShot; i++) {
		bullets.emplace_back(position, glm::rotate(direction, randSpread(rand)), m_bulletSpeed, m_bulletDamage);
	}
}

