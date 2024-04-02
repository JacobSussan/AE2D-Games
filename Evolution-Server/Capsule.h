#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include <vector>

class Capsule
{
//-------------------------------------------------------------------
	public:
	//-----------------------------------
	void init(std::vector<b2Body*> bodies);
	//-----------------------------------
	void setActiveBody(int& currentBodyIndex, int bodyIndex);
    //-----------------------------------
	b2Body* getBody(int index) { return m_bodies[index]; }
//-------------------------------------------------------------------
	private:
	std::vector<b2Body*> m_bodies;
};


