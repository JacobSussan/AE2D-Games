#pragma once
#include "Agent.h"

//-------------------------------------------------------------------
class Human : public Agent 
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 pos);
	virtual void update(const std::vector<std::string>& levelData,
						const std::vector<Human*>& humans,
						const std::vector<Zombie*>& zombies,
						float deltaTime) override;
private:
	int m_frames;
};

