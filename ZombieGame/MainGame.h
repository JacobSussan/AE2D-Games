#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "AE2D/Window.h"
#include "AE2D/GLSLProgram.h"
#include "AE2D/Camera2D.h"
#include "AE2D/InputManager.h"
#include "AE2D/SpriteBatch.h"
#include "AE2D/AudioEngine.h"
#include "AE2D/ParticleEngine2D.h"
#include "AE2D/ParticleBatch2D.h"
#include "AE2D/GUI.h"

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

//-------------------------------------------------------------------
enum class GameState {PLAY, EXIT};

//-------------------------------------------------------------------
class MainGame 
{
public:
    MainGame();
	~MainGame();
    /// Runs the game
    void run();

private:
    void initSystems();
	void initLevel();
    void initShaders();
	void initGUI();
    void gameLoop();
	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);
	void checkVictory();
    void processInput();
    void drawGame();
	void drawHud();
	void addBlood(const glm::vec2& pos, int numParticles);

    /// Member Variables
	CEGUI::PushButton  *m_infoButton;
	AE::GUI m_gui;
    AE::Window m_window;                 ///< The game window
    AE::GLSLProgram m_textureProgram;    ///< The shader program
    AE::InputManager m_inputManager;     ///< Handles input
	AE::Camera2D m_camera;               ///< Main Camera
	AE::Camera2D m_hudCamera;            ///< Hud Camera
	AE::SpriteBatch m_agentSpriteBatch;  ///< Draws agents
    AE::ParticleEngine2D m_particleEngine;
    AE::ParticleBatch2D* m_bloodParticleBatch;

	std::vector<Level*> m_levels;

	int m_screenWidth = 1280; 
	int m_screenHeight = 720;
	int m_currentLevel;
	float m_fps = 0.0f;
	float m_maxFPS = 120.0f;

	Player* m_player = nullptr;
	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombies;
	std::vector<Bullet> m_bullets;

	int m_numHumansKilled = 0;
	int m_numZombiesKilled = 0;

	AE::AudioEngine m_audioEngine;
	GameState m_gameState = GameState::PLAY;
};

