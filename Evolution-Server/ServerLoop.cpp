#include "ServerLoop.h"

#include "Light.h"

#include <iostream>
#include <fstream>
#include <string>
	
//-------------------------------------------------------------------
ServerLoop::ServerLoop()
{

}

//-------------------------------------------------------------------
ServerLoop::~ServerLoop()
{
	// destroy world
	for (unsigned long i = 0; i < m_TLBlocks.size(); i++) {
		m_TLBlocks[i].clear();
	}
	
	for (unsigned long i = 0; i < m_TRBlocks.size(); i++) {
		m_TRBlocks[i].clear();
	}
	
	for (unsigned long i = 0; i < m_BLBlocks.size(); i++) {
		m_BLBlocks[i].clear();
	}
	
	for (unsigned long i = 0; i < m_BRBlocks.size(); i++) {
		m_BRBlocks[i].clear();
	}

	m_TRBlocks.clear();
	m_TLBlocks.clear();
	m_BRBlocks.clear();
	m_BLBlocks.clear();
}

//-------------------------------------------------------------------
void ServerLoop::begin()
{
	m_limiter.setMaxFPS(MAX_TPS);
	bool m_isRunning = true;
	build();
	// GameLoop
	while (m_isRunning) {
		m_limiter.begin();
		update();
		m_tps = m_limiter.end();
	}
}

//-------------------------------------------------------------------
void ServerLoop::build()
{
	//-----------------------------------
	// make the world
	ReaderWriter::load("SaveGame", m_world, m_players, m_boxes, m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks);
}

//-------------------------------------------------------------------
void ServerLoop::destroy()
{

}

//-------------------------------------------------------------------
void ServerLoop::onExit()
{
	
}

//-------------------------------------------------------------------
void ServerLoop::update()
{
	// update all players
	for (unsigned long i = 0; i < m_players.size(); i++) {
		m_players[i].update(m_tps, m_boxes, m_blocks, m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks, m_world);

		glm::vec2 prevPlayerPos = m_players[i].getPrevPlayerPos();

		if (abs(prevPlayerPos.x) + 1 < abs(m_players[i].getPosition().x) 
	 	|| abs(prevPlayerPos.y) + 1 < abs(m_players[i].getPosition().y)
		|| abs(prevPlayerPos.x) - 1 > abs(m_players[i].getPosition().x) 
		|| abs(prevPlayerPos.y) - 1 > abs(m_players[i].getPosition().y) ) {
			updateActiveBodies();
			m_players[i].setPrevPlayerPos(m_players[i].getPosition());
		}

	}
	// Update physics sim
	float currentFPS = 60.0f;
	m_world->Step(1.0f / currentFPS, 20, 8);
	m_world->ClearForces();
	
	processPlayerInput();

	//TODO: check if there are any new players and let them join
	//TODO: check if any players have left and let them leave
	
	createUpdateFiles();
}

//-------------------------------------------------------------------
void ServerLoop::updateActiveBodies()
{
	// update all active bodies, keeping them all in the same world
	// TODO: Multiple worlds
	for (unsigned long i = 0; i < m_players.size(); i++) {
		glm::vec2 pPos;
		pPos.x = round(m_players[i].getPosition().x);
		pPos.y = round(m_players[i].getPosition().y);
		// loop m_blocks and if pos is too far away, disable the block
		for (unsigned long i = 0; i < m_blocks.size(); i++) {
			glm::vec2 blockPos;
			blockPos.x = m_blocks[i]->getBlock().getPos().x;
			blockPos.y = m_blocks[i]->getBlock().getPos().y;
	
			if (abs(abs(blockPos.x) - abs(pPos.x)) > 36 || abs(abs(blockPos.y) - abs(pPos.y)) > 24) {
				m_blocks[i]->getBlock().destroy(m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks);
				m_blocks.erase(m_blocks.begin() + i);
				//TODO: update blocks on either side of this block
			}
		}
	
		// ACTIVATE NEW | get player Pos
		// do some math to loop through Block_S's around the player
		// check if m_hasCounterpart = false && m_blockID != 0, create counterpart
		for (int i = pPos.y - 20; i < pPos.y + 20; i++) {
			for (int j = pPos.x - 33; j < pPos.x + 33; j++) {
				if (i >= 0) {		 		// top
					if (j >= 0) { 				// right
						if (!m_TRBlocks[i][j]->hasCounterpart() && m_TRBlocks[i][j]->getBlockID() != BLOCK_ID::AIR) {
							m_TRBlocks[i][j]->createCounterpart(m_world, m_blocks, glm::vec2(j, i));
							m_TRBlocks[i][j]->update(m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks);
						}
					} else if (j < 0) {			// left
						if (!m_TLBlocks[i][-j]->hasCounterpart() && m_TLBlocks[i][-j]->getBlockID() != BLOCK_ID::AIR) {
							m_TLBlocks[i][-j]->createCounterpart(m_world, m_blocks, glm::vec2(j, i));
							m_TLBlocks[i][-j]->update(m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks);
						}
					}
				} else if (i < 0) {			// bottom
					if (j >= 0) {				// right
						if (!m_BRBlocks[-i][j]->hasCounterpart() && m_BRBlocks[-i][j]->getBlockID() != BLOCK_ID::AIR) {
							m_BRBlocks[-i][j]->createCounterpart(m_world, m_blocks, glm::vec2(j, i));
							m_BRBlocks[-i][j]->update(m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks);
						}
					} else if (j < 0 ) {		// left
						if (!m_BLBlocks[-i][-j]->hasCounterpart() && m_BLBlocks[-i][-j]->getBlockID() != BLOCK_ID::AIR) {
							m_BLBlocks[-i][-j]->createCounterpart(m_world, m_blocks, glm::vec2(j, i));
							m_BLBlocks[-i][-j]->update(m_TLBlocks, m_TRBlocks, m_BLBlocks, m_BRBlocks);
						}
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------
void ServerLoop::playerJoin()
{
	Player player;
	//TODO: Handle player position, color, texture, etc
	player.init(m_world, 
					glm::vec2(0.0f, 10.0f),
					glm::vec2(2.0f, 2.0f),
					glm::vec2(1.1f, 1.8f),
					AE::Color_rgb8(255, 255, 255, 255));
					//-----------------------------------
	m_players.push_back(player);
}

//-------------------------------------------------------------------
void ServerLoop::playerLeave(unsigned long playerIndex)
{
	m_players.erase(m_players.begin() + playerIndex);
}

//-------------------------------------------------------------------
void ServerLoop::processPlayerInput()
{
	//TODO: Give each player a different id and use that instead of trusting the player to give you the correct id.
	for (unsigned long i = 0; i < m_players.size(); i++) {
		std::ifstream file("players/" + std::to_string(i));
		
		// set player mouse coords
		float tmpX, tmpY;
		file >> tmpX;
		file >> tmpY;
		m_players[i].getScreenMouseCoords() = glm::vec2(tmpX, tmpY);
		// set player buttons down
		unsigned int tmp;
		while(file >> tmp) {
			m_players[i].getPressedKeys().push_back(tmp);
		}
	}
}

//-------------------------------------------------------------------
void ServerLoop::createUpdateFiles() 
{
	//TODO: update only what needs to be updated
	std::ofstream file("updates/player");
}

