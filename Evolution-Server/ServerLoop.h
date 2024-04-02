#pragma once

#include <Box2D/Box2D.h>
#include "AE2D/Timing.h"

#include "Helper.h"
#include "ReaderWriter.h"

#include <vector>

class ServerLoop 
{
//-------------------------------------------------------------------
public:
	ServerLoop();
	~ServerLoop();
	
	void begin();
	void build();
	void destroy();
	void onExit();
	void update();
	void updateActiveBodies();
	void playerJoin();
	void playerLeave(unsigned long playerIndex);
	void processPlayerInput();
	void createUpdateFiles();
//-------------------------------------------------------------------
private:
	void checkInput();

	// Game things
	std::vector<Player> m_players;
	
	std::vector<Box> 		 m_boxes;				 // all boxes
	std::vector<Block_S*> 	 m_blocks;				 // active blocks
	std::vector< std::vector<Block_S*> > m_TLBlocks; // top left
	std::vector< std::vector<Block_S*> > m_TRBlocks; // top right
	std::vector< std::vector<Block_S*> > m_BLBlocks; // bottom left
	std::vector< std::vector<Block_S*> > m_BRBlocks; // bottom right
	b2World *m_world;
	
	int MAX_TPS = 60.0f;
	AE::FpsLimiter m_limiter;
	float m_tps = MAX_TPS;
	//-----------------------------------
};

