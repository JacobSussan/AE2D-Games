#include "MainGame.h"
#include "Gun.h"
#include "AE2D/AE.h"
#include "AE2D/Timing.h"
#include "AE2D/AEErrors.h"
#include "AE2D/ResourceManager.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <cstring>

//-------------------------------------------------------------------
const float HUMAN_SPEED    = 1.0f;
const float ZOMBIE_SPEED   = 1.3f;
const float PLAYER_SPEED   = 5.0f;

//-------------------------------------------------------------------
MainGame::MainGame()
{
	//EMPTY
}

//-------------------------------------------------------------------
MainGame::~MainGame()
{
	for (unsigned int i = 0; i < m_levels.size(); i ++ ) {
		delete m_levels[i];
	}
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	for (unsigned int i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}
}

//-------------------------------------------------------------------
void MainGame::run()
{
	initSystems();
	initLevel();
	AE::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg", 10);
	music.play(-1);
	gameLoop();
}

//-------------------------------------------------------------------
void MainGame::initSystems() 
{
	AE::init();

	m_audioEngine.init();

	m_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();
	m_agentSpriteBatch.init();

	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	m_bloodParticleBatch = new AE::ParticleBatch2D;
	m_bloodParticleBatch->init(1000, 0.05f, 
					AE::ResourceManager::getTexture("Textures/particle.png"), 
		[](AE::Particle2D& particle, float deltaTime) {
		particle.pos += particle.velocity * deltaTime;
		particle.color.a = (GLubyte)(particle.life * 255.0f);
	});

	m_particleEngine.addBatch(m_bloodParticleBatch);

	m_camera.setScale(0.5f);

	initGUI();
}

//-------------------------------------------------------------------
void MainGame::initLevel() 
{
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getPlayerPos(), 
				   &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	std::mt19937 rand;
	rand.seed(time(nullptr));
	std::uniform_int_distribution<int> randXPos(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randYPos(2, m_levels[m_currentLevel]->getHeight() - 2);

	//add humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
		m_humans.push_back(new Human);
		glm::vec2 pos(randXPos(rand) * TILE_WIDTH, randYPos(rand) * TILE_WIDTH);

		m_humans.back()->init(HUMAN_SPEED, pos);
	}

	//add zombies
	const std::vector<glm::vec2>& zombiePositions = 
			m_levels[m_currentLevel]->getZombiePos();

	for (unsigned int i = 0; i < zombiePositions.size(); i++) {
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_SPEED, 1000, 
		zombiePositions[i], 
		AE::ResourceManager::getTexture("Textures/zombie_mad.png").id);
	}

	//add guns
	m_player->addGun(new Gun("Magnum", 35, 1, 0.05f, 100, 20.0f, 
		m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav", 10)));

	m_player->addGun(new Gun("Shotgun", 30, 12, 0.2f, 30, 20.0f, 
		m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav", 10)));

	m_player->addGun(new Gun("MP5", 2, 1, 0.1f, 20, 20.0f, 
		m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav", 10)));

}

//-------------------------------------------------------------------
void MainGame::initShaders() 
{
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

//-------------------------------------------------------------------
void MainGame::initGUI()
{
	/// init UI
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.loadScheme("VanillaSkin.scheme");
	m_gui.setFont("DejaVuSans-10");

	m_infoButton = static_cast<CEGUI::PushButton*>
			(m_gui.createWidget("Vanilla/Button", glm::vec4(0.0f, 0.0f, 0.15f, 0.075f), glm::vec4(0.0f), "infoButton"));
}

//-------------------------------------------------------------------
void MainGame::gameLoop() 
{
	AE::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(m_maxFPS);

	const int MAX_PHYSICS_STEPS = 6;
	const float DESIRED_FPS = 60.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMTIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	Uint32 prevTicks = SDL_GetTicks();

	while (m_gameState == GameState::PLAY) {
		fpsLimiter.begin();

		Uint32 newTicks = SDL_GetTicks();
		Uint32 frameTime = newTicks - prevTicks;
		prevTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMTIME;

		checkVictory();
		m_inputManager.update();
		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}
		
		m_camera.setPosition(m_player->getPos());
		m_camera.update();
		m_hudCamera.update();
		drawGame();
		m_fps = fpsLimiter.end();
	}
}

//-------------------------------------------------------------------
void MainGame::updateAgents(float deltaTime) 
{
	// Update humans
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(), 
						m_humans, m_zombies, deltaTime);
	}

	// Update zombies
	for (unsigned int i = 0; i < m_zombies.size(); i++) {
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), 
						m_humans, m_zombies, deltaTime);
	}

	// Collide Zombies
	for (unsigned int i = 0; i < m_zombies.size(); i++) {
		// Z -> Z
		for (unsigned int j = i + 1; j < m_zombies.size(); j++) {
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}
		//Z -> H
		for (unsigned int j = 1; j < m_humans.size(); j++) {
			if (m_zombies[i]->collideWithAgent(m_humans[j])) {
				//add zombie
				m_zombies.push_back(new Zombie);
				static GLint textureID = AE::ResourceManager::getTexture
						("Textures/zombie.png").id;

				m_zombies.back()->init(ZOMBIE_SPEED, 100, 
								m_humans[j]->getPos(), textureID);

				//delete human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		//Z -> P
		if (m_zombies[i]->collideWithAgent(m_player)) {
			AE::fatalError("YOU LOSE!");
		}
	}

	// Collide Humans
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		// H -> H
		for (unsigned int j = i + 1; j < m_humans.size(); j++) {
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}

//-------------------------------------------------------------------
void MainGame::updateBullets(float deltaTime) 
{
	for (unsigned int i = 0; i < m_bullets.size();) {
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(),
							   	deltaTime)) {
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		} else {
			i++;
		}
	}

	bool wasBulletRemoved;
	for (unsigned int i = 0; i < m_bullets.size(); i++) {
		wasBulletRemoved = false;
		
		//zombies
		for (unsigned int j = 0; j < m_zombies.size(); ) {
			if (m_bullets[i].collideWithAgent(m_zombies[j])) {
				addBlood(m_bullets[i].getPos(), 10);

				//damage zombie
				if (m_zombies[j]->damage(m_bullets[i].getDamage())) {
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				} else j++;

				//remove bullet
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				wasBulletRemoved = true;
				i--;
				break;
			} else j++;
		}

		if (wasBulletRemoved == false) {
			//humans
			for (unsigned int j = 1; j < m_humans.size();) {
				if (m_bullets[i].collideWithAgent(m_humans[j])) {
					addBlood(m_bullets[i].getPos(), 10);

					//damage human
					if (m_humans[j]->damage(m_bullets[i].getDamage())) {
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
						m_numHumansKilled++;
					} else j++;

					//remove bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					wasBulletRemoved = true;
					i--;
					break;
				} else j++;
			}
		}
	}
}

//-------------------------------------------------------------------
void MainGame::checkVictory() 
{
	if (m_zombies.empty()) 
	{
		std::printf("*** YOU WIN! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining.\n\n\n",
					m_numHumansKilled,
					m_numZombiesKilled,
					(int)m_humans.size() - 1,
					m_levels[m_currentLevel]->getNumHumans());
		AE::fatalError("YOU WIN!");
	}
}

//-------------------------------------------------------------------
void MainGame::processInput() 
{
    SDL_Event evnt;
	// Keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {     
        switch (evnt.type) {
            case SDL_QUIT:
				m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

//-------------------------------------------------------------------
void MainGame::drawGame() 
{
	glClearDepth(1.0);   // Set the base depth to 1.0

	// Clear the Color_rgb8 and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);     // draw code

	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");

	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_levels[m_currentLevel]->draw();
	m_agentSpriteBatch.begin();
	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	//draw humans
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		if (m_camera.isBoxInView(m_humans[i]->getPos(), agentDims)) {
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}

	//draw humans
	for (unsigned int i = 0; i < m_zombies.size(); i++) {
		if (m_camera.isBoxInView(m_zombies[i]->getPos(), agentDims)) {
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	//draw bullets
	for (unsigned int i = 0; i < m_bullets.size(); i++) {
		if (m_camera.isBoxInView(m_bullets[i].getPos(), agentDims)) {
			m_bullets[i].draw(m_agentSpriteBatch);
		}
	}

	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();
	m_particleEngine.draw(&m_agentSpriteBatch);

	drawHud();

	// Swap our buffer and draw everything to the screen!
    m_window.swapBuffer();     
	
	m_textureProgram.unuse();
}

//-------------------------------------------------------------------
void MainGame::drawHud() 
{
	static int frameCounter = 0;
	static int estimatedFPS = 60;
	frameCounter++;
	if (frameCounter >= m_fps / 2) {
		estimatedFPS = m_fps;
		frameCounter = 0;
	}

	m_infoButton->setText("FPS: " + std::to_string(estimatedFPS) + "/ 120 \n Humans: " 
					+ std::to_string((int)m_humans.size()) + "\n Zombies: " + std::to_string(m_zombies.size()));
	
	m_gui.draw();
	glEnable(GL_BLEND);	
}

//-------------------------------------------------------------------
void MainGame::addBlood(const glm::vec2& pos, int numParticles) 
{
	static std::mt19937 rand(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);
	glm::vec2 vel(2.0f, 0.0f);
	AE::Color_rgb8 col(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++) {
		m_bloodParticleBatch->addParticle(pos, 
						glm::rotate(vel, randAngle(rand)), 
						col, 15.0f);
	}
}


