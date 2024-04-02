#pragma once

#include "../Block.h"

class Block_S;

//-------------------------------------------------------------------
class Glass_S : public Block_S
{
public:
	virtual void init() override;
	virtual void createCounterpart(b2World *world, 
						std::vector<Block_S*> &blocks,
						const glm::vec2 &pos) override;

};

