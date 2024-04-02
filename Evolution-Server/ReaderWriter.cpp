#include "ReaderWriter.h"

#include "AE2D/ResourceManager.h"
#include "AE2D/AEErrors.h"
#include "Helper.h"

#include <fstream>
#include <iostream>

const float GAME_VERSION_0 = 0.001;

bool ReaderWriter::save(
			const std::string &filePath,
			std::vector<Player> &players,
			const std::vector<Box> &boxes,
			std::vector< std::vector<Block_S*> > &TLBlocks,
			std::vector< std::vector<Block_S*> > &TRBlocks,
			std::vector< std::vector<Block_S*> > &BLBlocks,
			std::vector< std::vector<Block_S*> > &BRBlocks ) 
			//-----------------------------------
{
	// open file
	std::ofstream file(filePath);
	if (file.fail()) {
		AE::fatalError("Could not save " + filePath + "\n");
		return false;
	}

	// save game version
	file << GAME_VERSION_0 << "\n";

	// save all the players
	file << players.size() << "\n";
	for (unsigned long i = 0; i < players.size(); i++) {
		file << players[i].getPosition().x << " " << players[i].getPosition().y << " "
			<< players[i].getColor().r << " " << players[i].getColor().g << " "
			<< players[i].getColor().b << " " << players[i].getColor().a << " "
			<< players[i].getTextureFilePath() << "\n";
	}
	// save boxes
	file << boxes.size() << "\n";
	for (auto b : boxes) {
		file << b.getBody()->GetPosition().x << " " << b.getBody()->GetPosition().y << " "
				<< b.getDimentions().x << " " <<  b.getDimentions().y << " "
				<< b.getColor().r << " " << b.getColor().g << " "
				<< b.getColor().b << " " << b.getColor().a << " "
				<< b.getUvRect().x << " " << b.getUvRect().y << " "
				<< b.getUvRect().z << " " << b.getUvRect().w << " "
				<< b.getBody()->GetAngle() << " " << b.getTexture().filePath << " "
				<< b.isDynamic() << " " << b.getFixedRotation() << "\n";
	}

	file  << TLBlocks.size();
	// save blocks
	for (unsigned long i = 0; i < TLBlocks.size(); i++) {
		file << "\n" << TLBlocks[i].size() << " ";
		for (unsigned long j = 0; j < TLBlocks[i].size(); j++) {
			file << (unsigned int)TLBlocks[i][j]->getBlockID() << " ";
		}
	}
	file << std::endl;
	
	file << TRBlocks.size();
	for (unsigned long i = 0; i < TRBlocks.size(); i++) {
		file << "\n" << TRBlocks[i].size() << " ";
		for (unsigned long j = 0; j < TRBlocks[i].size(); j++) {
			file << (unsigned int)TRBlocks[i][j]->getBlockID() << " ";
		}	
	}
	file << std::endl;

	file << BLBlocks.size();
	for (unsigned long i = 0; i < BLBlocks.size(); i++) {
		file << "\n" << BLBlocks[i].size() << " ";
		for (unsigned long j = 0; j < BLBlocks[i].size(); j++) {
			file << (unsigned int)BLBlocks[i][j]->getBlockID() << " ";
		}	
	}
	file << std::endl;

	file << BRBlocks.size();
	for (unsigned long i = 0; i < BRBlocks.size(); i++) {
		file << "\n" << BRBlocks[i].size() << " ";
		for (unsigned long j = 0; j < BRBlocks[i].size(); j++) {
			file << (unsigned int)BRBlocks[i][j]->getBlockID() << " ";
		}	
	}
	file << std::endl;

	return true;
}

//-------------------------------------------------------------------
bool ReaderWriter::load(
			const std::string &filePath,
			b2World *&world,
			std::vector<Player> &players,
			std::vector<Box> &boxes,
			std::vector< std::vector<Block_S*> > &TLBlocks,
			std::vector< std::vector<Block_S*> > &TRBlocks,
			std::vector< std::vector<Block_S*> > &BLBlocks,
			std::vector< std::vector<Block_S*> > &BRBlocks ) 
			//-----------------------------------
{	
	std::ifstream file(filePath);
	if (file.fail()) {
		AE::fatalError("Could not load file " + filePath + "\n");
		return false;
	}

	// destroy world
	delete world;

	// make new world	
	b2Vec2 gravity(0.0f, -40.0f); 
	world = new b2World(gravity);
	
	// clear everything
	players.clear();
	boxes.clear();
	// top left
	for (unsigned int i = TLBlocks.size(); i > 0; i--) {
		for (unsigned int j = TLBlocks[i].size(); j > 0; j--) {
			TLBlocks[i][j]->~Block_S();
		}
		TLBlocks[i].clear();
	}
	TLBlocks.clear();
	// top right
	for (unsigned int i = TRBlocks.size(); i > 0; i--) {
		for (unsigned int j = TRBlocks[i].size(); j > 0; j--) {
			TRBlocks[i][j]->~Block_S();
		}
	}
	// bottom left
	for (unsigned int i = BLBlocks.size(); i > 0; i--) {
		for (unsigned int j = BLBlocks[i].size(); j > 0; j--) {
			BLBlocks[i][j]->~Block_S();
		}
		BLBlocks[i].clear();
	}
	BLBlocks.clear();
	// bottom right
	for (unsigned int i = BRBlocks.size(); i > 0; i--) {
		for (unsigned int j = BRBlocks[i].size(); j > 0; j--) {
			BRBlocks[i][j]->~Block_S();
		}
		BRBlocks[i].clear();
	}
	BRBlocks.clear();

	// load everything from file
	float version;
	file >> version;

	// read players
	float numPlayers;
	file >> numPlayers;
	players.resize(numPlayers);		
	
	glm::vec2 pos;
	float r, g, b, a;
	std::string textureFilePath;
	for (unsigned int i = 0; i < numPlayers; i++) {
		file >> pos.x >> pos.y >> r >> g >> b >> a >> textureFilePath;
		players[i].init(world, pos, glm::vec2(2.0, 2.0), glm::vec2(1.1f, 1.8f), AE::Color_rgb8(r, g, b, a));
	}

	// read boxes
	unsigned int numBoxes; 
	file >> numBoxes;
	
	glm::vec2 dims;
	glm::vec4 uvRect;
	bool fixedRotation;
	bool isDynamic;
	float angle;
	AE::GLTexture texture;
	
	for (unsigned int i = 0; i < numBoxes; i++) {
		file >> pos.x >> pos.y >> dims.x >> dims.y >> r >> g >> b >> a
				>> uvRect.x >> uvRect.y >> uvRect.z >> uvRect.w >> angle >> textureFilePath >> isDynamic >> fixedRotation;
		texture = AE::ResourceManager::getTexture(textureFilePath);
		
		boxes.emplace_back();
		boxes.back().init(world, pos, dims, texture, AE::Color_rgb8(r, g, b, a), 
						fixedRotation, isDynamic, angle, uvRect);

	}

	unsigned int tmp1, tmp2;
	unsigned int blockID;
	BLOCK_ID Block_ID;
	
	// Top left
	file >> tmp1;
	TLBlocks.resize(tmp1);
	for (unsigned int i = 0; i < tmp1; i++) {
		TLBlocks[i] = std::vector<Block_S*>();
		file >> tmp2;
		for (unsigned int j = 0; j < tmp2; j++) {
			TLBlocks[i].resize(tmp2);
			file >> blockID;
			Block_ID = static_cast<BLOCK_ID>(blockID);
			Block_S *newBlock_S = Helper::getBlockTypeFromID(Block_ID);

			newBlock_S->init();
			TLBlocks[i][j] = newBlock_S; 
		}
	}

	// Top Right
	file >> tmp1;
	TRBlocks.resize(tmp1);
	for (unsigned int i = 0; i < tmp1; i++) {
		TRBlocks[i] = std::vector<Block_S*>();
		file >> tmp2;
		for (unsigned int j = 0; j < tmp2; j++) {
			TRBlocks[i].reserve(tmp2);
			file >> blockID;
			Block_ID = static_cast<BLOCK_ID>(blockID);
			Block_S *newBlock_S = Helper::getBlockTypeFromID(Block_ID);

			newBlock_S->init();
			TRBlocks[i][j] = newBlock_S; 
		}
	}
	
	// Bottom Left
	file >> tmp1;
	BLBlocks.resize(tmp1);
	for (unsigned int i = 0; i < tmp1; i++) {
		BLBlocks[i] = std::vector<Block_S*>();
		file >> tmp2;
		for (unsigned int j = 0; j < tmp2; j++) {
			BLBlocks[i].reserve(tmp2);
			file >> blockID;
			Block_ID = static_cast<BLOCK_ID>(blockID);
			Block_S *newBlock_S = Helper::getBlockTypeFromID(Block_ID);

			newBlock_S->init();
			BLBlocks[i][j] = newBlock_S; 
		}
	}

	// Bottom Right 
	file >> tmp1;
	BRBlocks.resize(tmp1);
	for (unsigned int i = 0; i < tmp1; i++) {
		BRBlocks[i] = std::vector<Block_S*>();
		file >> tmp2;
		for (unsigned int j = 0; j < tmp2; j++) {
			BRBlocks[i].reserve(tmp2);
			file >> blockID;
			Block_ID = static_cast<BLOCK_ID>(blockID);
			Block_S *newBlock_S = Helper::getBlockTypeFromID(Block_ID);

			newBlock_S->init();
			BRBlocks[i][j] = newBlock_S; 
		}
	}

	return true;
}


