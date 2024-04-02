#pragma once

#include <vector>

#include "Ball.h"

class Grid;

//-------------------------------------------------------------------
enum class GravityDirection {NONE, LEFT, UP, RIGHT, DOWN};

//-------------------------------------------------------------------
class BallController
{
public:
    void updateBalls(std::vector <Ball>& balls, Grid* grid, float deltaTime, int maxX, int maxY);

    /// Event functions
    void onMouseDown(std::vector <Ball>& balls, float mouseX, float mouseY);
    void onMouseUp(std::vector <Ball>& balls);
    void onMouseMove(std::vector <Ball>& balls, float mouseX, float mouseY);
    void setGravityDirection(GravityDirection dir) { m_gravityDirection = dir; }
private:
	void updateCollision(Grid* grid);                 ///< Updates collision of the cells
	void checkCollision(Ball& b1, Ball& b2);          ///< Checks collision between two balls
	
	// Checks collision between a ball and a vector starting an index
	void checkCollision(Ball* ball, std::vector<Ball*>& ballsToCheck, int startingIndex);  
 	
	//Returns true if the mouse is hovering over a ball
    bool isMouseOnBall(Ball&b, float mouseX, float mouseY);
    glm::vec2 getGravityAccel();

    int m_grabbedBall = -1;                   ///< The ball we are currently grabbing on to
    glm::vec2 m_prevPos = glm::vec2(0.0f);    ///< Previous position of the grabbed ball
    glm::vec2 m_grabOffset = glm::vec2(0.0f); ///< Offset of the cursor on the selected ball

    GravityDirection m_gravityDirection = GravityDirection::NONE;
};

