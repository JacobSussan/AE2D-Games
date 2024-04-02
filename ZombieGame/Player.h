#pragma once
#include "Human.h"
#include "Bullet.h"

#include "AE2D/InputManager.h"
#include "AE2D/Camera2D.h"

class Gun;

//-------------------------------------------------------------------
class Player : public Human 
{
public:
	Player();
	~Player();

	void init(float speed, glm::vec2 pos, 
			  AE::InputManager* inputManager, 
			  AE::Camera2D* camera, std::vector<Bullet>* bullets);	
	void addGun(Gun* gun);
	void update(const std::vector<std::string>& levelData,
				const std::vector<Human*>& humans,
				const std::vector<Zombie*>& zombies,
				float deltaTime) override;

private:
	AE::InputManager* m_inputManager;
	std::vector<Gun*> _guns;
	int _currentGunIndex;
	AE::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;
};

