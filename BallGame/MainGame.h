#pragma once

#include "AE2D/Camera2D.h"
#include "AE2D/SpriteBatch.h"
#include "AE2D/InputManager.h"
#include "AE2D/Window.h"
#include "AE2D/GLSLProgram.h"
#include "AE2D/Timing.h"
#include <memory>

#include "BallController.h"
#include "BallRenderer.h"
#include "Grid.h"

const int CELL_SIZE = 12;

//-------------------------------------------------------------------
enum class GameState { RUNNING, EXIT };

//-------------------------------------------------------------------
class MainGame 
{
public:
	~MainGame();
    void run();

private:
    void init();
	void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

	std::vector<Ball> m_balls;                          ///< All the balls
	std::unique_ptr<Grid> m_grid;                       ///< Grid for spatial partitioning / collision
   
   	int m_currentRenderer = 0;	
	std::vector<BallRenderer* > m_ballRenderers;
	
	BallController m_ballController;                    ///< Controls balls
    AE::Window m_window;                         	    ///< The main window
    AE::SpriteBatch m_spriteBatch;                    	///< Renders all the balls
    AE::Camera2D m_camera;                              ///< Renders the scene
    AE::InputManager m_inputManager;                    ///< Handles input
    AE::GLSLProgram m_textureProgram;                   ///< Shader for textures
    AE::FpsLimiter m_fpsLimiter;                        ///< Limits and calculates fps
    GameState m_gameState = GameState::RUNNING;         ///< The state of the game
	float m_fps = 0.0f;
};

