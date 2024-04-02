#pragma once
#include "Agent.h"

//-------------------------------------------------------------------
class Zombie : public Agent 
{
public:
	Zombie();
	~Zombie();

	void init(float speed, float health, 
					glm::vec2 pos, GLuint textureID);
	void update(const std::vector<std::string>& levelData,
				const std::vector<Human*>& humans,
				const std::vector<Zombie*>& zombies,
				float deltaTime) override;
private:
	Human* getNearestHuman(const std::vector<Human*>& humans);
	int m_frames;
};

