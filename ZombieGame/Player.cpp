#include "Player.h"
#include "Gun.h"
#include "AE2D/ResourceManager.h"

#include <SDL/SDL.h>

Player::Player() : 
	_currentGunIndex(-1) 
{
//empty
}


Player::~Player() 
{
//empty
}

void Player::init(float speed, glm::vec2 pos, AE::InputManager* inputManager, AE::Camera2D* camera, std::vector<Bullet>* bullets) 
{
	m_health = 100.0f;
	m_bullets = bullets;
	m_speed = speed;
	m_position = pos;
	m_inputManager = inputManager;
	m_camera = camera;
	m_color = AE::Color_rgb8(255, 255, 255, 255);
	m_textureID = AE::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);
	//if no gun is equiped, equpid it
	if (_currentGunIndex == -1) {
		_currentGunIndex = 0;
	}
}

void Player::update(const std::vector<std::string>& levelData,
					const std::vector<Human*>& humans,
					const std::vector<Zombie*>& zombies, 
					float deltaTime)
{
	if (m_inputManager->isKeyDown(SDLK_w)) {
		if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_d)) {
			m_position.y += ((m_speed * deltaTime) / 1.5);
		} else {
			m_position.y += (m_speed * deltaTime);
		}
	} else if (m_inputManager->isKeyDown(SDLK_s)) {
		if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_d)) {
			m_position.y -= ((m_speed * deltaTime) / 1.5);
		} else {
			m_position.y -= (m_speed * deltaTime);
		}
	}
	if (m_inputManager->isKeyDown(SDLK_a)) {
		if (m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_s)) {
			m_position.x -= ((m_speed * deltaTime) / 1.5);
		} else {
			m_position.x -= (m_speed * deltaTime);
		}
	} else if (m_inputManager->isKeyDown(SDLK_d)) {
		if (m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_s)) {
			m_position.x += ((m_speed * deltaTime) / 1.5);
		} else {
			m_position.x += (m_speed * deltaTime);
		}
	}

	if (m_inputManager->isKeyDown(SDLK_1)) {
		_currentGunIndex = 0;
	} else if (m_inputManager->isKeyDown(SDLK_2) && _guns.size() >= 1) {
		_currentGunIndex = 1;
	} else if (m_inputManager->isKeyDown(SDLK_3) && _guns.size() >= 2) {
		_currentGunIndex = 2;
	}

	glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
	mouseCoords = m_camera->convertScreenToWorld(mouseCoords);

	glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);

	m_direction = glm::normalize(mouseCoords - centerPosition);

	if (_currentGunIndex != -1) {
		_guns[_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
										centerPosition,
										m_direction,
										*m_bullets,
										deltaTime);

	}
	collideWithLevel(levelData);
}
