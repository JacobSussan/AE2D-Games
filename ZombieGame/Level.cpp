#include "Level.h"
#include "AE2D/AEErrors.h"

#include <fstream>
#include <iostream>

//-------------------------------------------------------------------
Level::Level(const std::string& fileName) 
{
	std::ifstream file;
	file.open(fileName);
	if (file.fail()) { AE::fatalError("Failed to open " + fileName); }

	std::string tmp;
	file >> tmp >> _numHumans;

	std::getline(file, tmp);

	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}	

	_spriteBatch.init();

	_spriteBatch.begin();

	glm::vec4 uvRect(0, 0, 1, 1);
	AE::Color_rgb8 whiteColor(255, 255, 255, 255);

	for (int y = 0; y < (int)_levelData.size(); y++) {
		for (int x = 0; x < (int)_levelData[y].size() - 1; x++) {
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);
			
			switch (_levelData[y][x]) {
			case 'R':
			case 'B':
				_spriteBatch.draw(destRect, 
								  uvRect, 
								  AE::ResourceManager::getTexture("Textures/red_bricks.png").id,
								  0.0f,
								  whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect,
								  uvRect,
								  AE::ResourceManager::getTexture("Textures/glass.png").id,
								  0.0f,
								  whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect,
								  uvRect,
								  AE::ResourceManager::getTexture("Textures/light_bricks.png").id,
								  0.0f,
								  whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';
				_playerPos.x = x * TILE_WIDTH;
				_playerPos.y = y * TILE_WIDTH;
				break;

			case 'Z':
				_levelData[y][x] = '.';
				_zombiePos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::cout << "Unexpected symbol in save data!\n";
				std::cout << _levelData[y][x] << std::endl;
				break;
			}
		}
	}
	_spriteBatch.end();
}

//-------------------------------------------------------------------
Level::~Level() 
{

}

//-------------------------------------------------------------------
void Level::draw() 
{
	_spriteBatch.renderBatch();
}
