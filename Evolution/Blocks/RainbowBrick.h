#pragma once

#include "../Block.h"

class Block;

//-------------------------------------------------------------------
class RainbowBrick : public Block
{
public:
	virtual void init() override;

private:
	AE::Color_rgb8 m_color;

};

