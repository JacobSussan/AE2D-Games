#pragma once

#include <string>
#include "Player.h"
#include "Block.h"
#include "Box.h"

//-------------------------------------------------------------------

class ReaderWriter 
{
	public:
	static bool save(
			const std::string &filePath,
			std::vector<Player> &players,
			const std::vector<Box> &boxes,
			std::vector< std::vector<Block_S*> > &TLBlocks,
			std::vector< std::vector<Block_S*> > &TRBlocks,
			std::vector< std::vector<Block_S*> > &BLBlocks,
			std::vector< std::vector<Block_S*> > &BRBlocks );
	//-----------------------------------
	static bool load(
			const std::string &filePath,
			b2World *&world,
			std::vector<Player> &players,
			std::vector<Box> &boxes,
			std::vector< std::vector<Block_S*> > &TLBlocks,
			std::vector< std::vector<Block_S*> > &TRBlocks,
			std::vector< std::vector<Block_S*> > &BLBlocks,
			std::vector< std::vector<Block_S*> > &BRBlocks );
};

