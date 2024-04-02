#pragma once

#include <vector>
#include <string>

#include "AE2D/SpriteBatch.h"
#include "AE2D/ResourceManager.h"

const int TILE_WIDTH = 64;

//-------------------------------------------------------------------
class Level 
{
public:
	Level(const std::string& fileName);
	~Level();

	void draw();

	//get
	int getWidth() const { return _levelData[0].size(); }
	int getHeight() const { return _levelData.size(); }
	int getNumHumans() const { return _numHumans; }
	const std::vector<std::string>& getLevelData() const { return _levelData; }
	glm::vec2 getPlayerPos() const { return _playerPos; }
	const std::vector<glm::vec2>& getZombiePos() const { return _zombiePos; }

private:
	int _numHumans;
	glm::vec2 _playerPos;
	std::vector<std::string> _levelData;
	AE::SpriteBatch _spriteBatch;
	std::vector<glm::vec2> _zombiePos;
};

