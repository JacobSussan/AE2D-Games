#pragma once

#include <string>
#include "Player.h"
#include "Block.h"
#include "Box.h"
#include "Helper.h"

//-------------------------------------------------------------------

class ReaderWriter 
{
	public:
	static bool save(
			const std::string &filePath,
			std::vector<Player> &players,
			std::vector<Box> &boxes);
	//-----------------------------------
	static bool load(
			const std::string &filePath,
			b2World *&world,
			std::vector<Player> &players,
			std::vector<Box> &boxes);
	//-----------------------------------
	static bool loadChunk(b2World *&world, 
					std::vector<chunk_S> *cList, 
					chunk_S *c);
	//-----------------------------------
	static bool saveChunk(chunk_S* c);
};

