#include "ReaderWriter.h"

#include "AE2D/ResourceManager.h"
#include "AE2D/AEErrors.h"

#include <fstream>
#include <iostream>

const float GAME_VERSION_0 = 0.001;

bool ReaderWriter::save(
			const std::string &filePath,
			std::vector<Player> &players,
			std::vector<Box> &boxes) 
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
			<< (float)players[i].getColor().r << " " << (float)players[i].getColor().g << " "
			<< (float)players[i].getColor().b << " " << (float)players[i].getColor().a << " "
			<< players[i].getTextureFilePath() << "\n";
	}
	// save boxes
	file << boxes.size() << "\n";
	for (auto b : boxes) {
		file << b.getBody()->GetPosition().x << " " << b.getBody()->GetPosition().y << " "
				<< b.getDimentions().x << " " <<  b.getDimentions().y << " "
				<< (float)b.getColor().r << " " << (float)b.getColor().g << " "
				<< (float)b.getColor().b << " " << (float)b.getColor().a << " "
				<< b.getUvRect().x << " " << b.getUvRect().y << " "
				<< b.getUvRect().z << " " << b.getUvRect().w << " "
				<< b.getBody()->GetAngle() << " " << b.getTexture().filePath << " "
				<< b.isDynamic() << " " << b.getFixedRotation() << "\n";
	}

	// TODO: save active chunks

	return true;
}

//-------------------------------------------------------------------
bool ReaderWriter::load(
			const std::string &filePath,
			b2World *&world,
			std::vector<Player> &players,
			std::vector<Box> &boxes) 
			//-----------------------------------
{	
	std::ifstream file(filePath);
	if (file.fail()) {
		AE::fatalError("Could not load file " + filePath + "\n");
		return false;
	}

	// destroy world
	if (world) {
		delete world;
	}
	// make new world	
	b2Vec2 gravity(0.0f, 0.0f); // 0.0f, -40.0f
	world = new b2World(gravity);
	
	// clear everything
	players.clear();
	boxes.clear();
	
	//TODO: add players and boxes last.
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

	return true;
}

//-------------------------------------------------------------------
bool ReaderWriter::loadChunk(b2World *&world, std::vector<chunk_S> *cList, chunk_S *c) 
{
	// loop loadedChunkPos and if pos = any pos in there, return false
	for (unsigned long i = 0; i < cList->size(); i++) {
		if ((*cList)[i].m_pos == c->m_pos) {
			return false;
		}
	}	
	// if pos != any pos in loadedChunkPos, create the chunk and return true
	// active chunk
	//c->m_chunk->activate();
	// open Save1/0_0_0 (quad, x, y)
	int quadInt = (int)c->m_chunk->getQuad();
	
	std::ifstream file("Save1/" + std::to_string(quadInt) + "_" + 
					std::to_string((int)c->m_pos.x) + "_" + 
					std::to_string((int)c->m_pos.y));

	unsigned int blockID;

	std::cout << "- Open: Save1/" 
			+ std::to_string(quadInt) + "_" 
			+ std::to_string((int)c->m_pos.x) + "_" 
			+ std::to_string((int)c->m_pos.y) + "\n";

	// TODO: Return a new chunk or something? 
	for (unsigned long i = 0; i < 16; i++) {
		for (unsigned long j = 0; j < 16; j++) {
			file >> blockID;
			//std::cout << "\n Block ID: " + std::to_string(blockID) + "\n";
			BLOCK_ID Block_ID = static_cast<BLOCK_ID>(blockID);
			Block *newBlock = Helper::getBlockTypeFromID(Block_ID);
			newBlock->init();
			//c->m_chunk->setBlock(newBlock, j, i);
		}
	}
	c->m_chunk->createBodies(world, c->m_pos);
	cList->push_back(*c);
	return true;
}

//-------------------------------------------------------------------
bool ReaderWriter::saveChunk(chunk_S* c)
{
	std::ofstream file("Save1/" + std::to_string((int)c->m_chunk->getQuad()) + "_" + 
					std::to_string((int)c->m_pos.x) + 
					"_" + std::to_string((int)c->m_pos.y));
	
	// loop all blocks and save the id
	
	std::vector<std::vector<Block* > > *blocks = c->m_chunk->getBlocks();

	for (unsigned long i = 0; i < blocks->size(); i++) {
		for (unsigned long j = 0; j < blocks[i].size(); j++) {
			file << (int)(*blocks)[i][j]->getBlockID() << " ";
		}
		file << "\n";
	}
	return true;
}

