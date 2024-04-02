#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "AE2D/AudioEngine.h"
#include "Bullet.h"

//-------------------------------------------------------------------
class Gun 
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed, AE::SoundEffect fireSound);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

private:
	AE::SoundEffect m_fireSound;
	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);
	std::string m_name;


	int m_fireRate;
	int m_bulletsPerShot;
	float m_spread;

	//Bullets
	float m_bulletSpeed;
	float m_bulletDamage;
	float m_frameCounter;
};

