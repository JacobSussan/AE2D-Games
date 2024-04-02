#pragma once

#include "Ball.h"
#include <vector>

//-------------------------------------------------------------------
struct Cell 
{
	std::vector<Ball*> balls;
};

//-------------------------------------------------------------------
class Grid 
{
	friend class BallController;
public:
	Grid(int width, int height, int cellSize);
	~Grid();

	void addBall(Ball* ball);              ///< If we don't already have the cell
	void addBall(Ball* ball, Cell* cell);  ///< If we already have the cell
	Cell* getCell(int x, int y);           ///< Gets cell from cell cords
	Cell* getCell(const glm::vec2 pos);    ///< Gets cell from window cords

	void removeBallFromCell(Ball* ball);

private:
	std::vector<Cell> m_cells;
	int m_cellSize, m_width, m_height, m_numXCells, m_numYCells;
};

