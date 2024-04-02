#include "GameplayScreen.h"

#include "Light.h"
#include "ScreenIndices.h"
#include "ReaderWriter.h"

#ifdef CLIENT
	#include "AE2D/IMainGame.h"
	#include "AE2D/ResourceManager.h"

	#include <SDL2/SDL.h>
#endif

#include <iostream>
#include <string>
#include <fstream>

#ifdef CLIENT
//-------------------------------------------------------------------
GameplayScreen::GameplayScreen(AE::Window* window) : m_window(window) 
{ 
	m_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

#else
//-------------------------------------------------------------------
GameplayScreen::GameplayScreen() { }
#endif
//-------------------------------------------------------------------
GameplayScreen::~GameplayScreen()
{
	// remove all chunks
	
	for (unsigned long i = 0; i < m_chunks.size(); i++) {
		m_chunks[i].m_chunk->deactivate();
	}
}

#ifdef CLIENT
//-------------------------------------------------------------------
int GameplayScreen::getNextScreenIndex() const 
{
	return SCREEN_INDEX_NO_SCREEN;
}

//-------------------------------------------------------------------
int GameplayScreen::getPreviousScreenIndex() const 
{
	return SCREEN_INDEX_MAIN_MENU;
}
#endif

//-------------------------------------------------------------------
void GameplayScreen::build()
{

}

//-------------------------------------------------------------------
void GameplayScreen::destroy()
{

}

#ifdef CLIENT
//-------------------------------------------------------------------
void GameplayScreen::onEntry()
{	
	// loads the game
	ReaderWriter::load("SaveGame", m_world, m_players, m_boxes);

	//-----------------------------------
	// init the debug renderer
	m_debugRenderer.init();
	// init spritebach
	m_spriteBatch.init();

	// init shaders
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	m_lightProgram.compileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
	m_lightProgram.addAttribute("vertexPosition");
	m_lightProgram.addAttribute("vertexColor");
	m_lightProgram.addAttribute("vertexUV");
	m_lightProgram.linkShaders();

	// init camera
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeighth());
	m_camera.setScale(25);

	initGUI();
}

#else
//-------------------------------------------------------------------
// Only for the server.
void GameplayScreen::begin() 
{
	// load game
	ReaderWriter::load("SaveGame", m_world, m_players, m_boxes);
	// set server tps 
	m_limiter.setMaxFPS(MAX_TPS);
	bool isRunning = true;
	build();
	// main loop
	while (isRunning) {
		m_limiter.begin();
		update();
		m_tps = m_limiter.end();
	}
}
#endif

#ifdef CLIENT
//-------------------------------------------------------------------
void GameplayScreen::initGUI()
{
	/// init UI
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.loadScheme("VanillaSkin.scheme");
	m_gui.setFont("DejaVuSans-10");

	// debug button for player pos and fps
	m_debugButton = static_cast<CEGUI::PushButton*>
			(m_gui.createWidget("Vanilla/Button", glm::vec4(0.0f, 0.0f, 0.15f, 0.075f), glm::vec4(0.0f), "DebugButton"));
	
	// set event to be called on click
	m_debugButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
							CEGUI::Event::Subscriber(&GameplayScreen::onDebugButtonClick, this));
}

//-------------------------------------------------------------------
void GameplayScreen::onExit()
{
	m_debugRenderer.dispose();
}
#endif

//-------------------------------------------------------------------
void GameplayScreen::update()
{
#ifdef CLIENT

	// center player on screen
	m_camera.setPosition(m_players[0].getPosition());
	m_camera.update();

	checkInput();

	// clear current pressed keys vector
	m_players[0].getPressedKeys().clear();
	// copy keymap from inputmanager to local keymap
	auto keyMap = m_game->inputManager.getKeyMap();
	for (auto it = keyMap.begin(); it != keyMap.end(); ++it) {
		if (it->second == true) {
			m_players[0].getPressedKeys().push_back(*&it->first);
		}
	}

	// saving loading test
	for (unsigned int i = 0; i < m_players[0].getPressedKeys().size(); i++) {
		// save game to file
		if (m_players[0].getPressedKeys()[i] == SDLK_F5) {
			ReaderWriter::save("SaveGame", m_players, m_boxes);
		} else if (m_players[0].getPressedKeys()[i] == SDLK_F6) {
			// load game from file
			// TODO: broken...
			// re make m_blocks list
			for (unsigned int i = 0; i < m_chunks.size(); i++) {
				delete m_chunks[i].m_chunk;
			}
			m_chunks.clear();
			ReaderWriter::load("SaveGame", m_world, m_players, m_boxes);
			updateActiveBodies();
		}
	}

	m_players[0].getScreenMouseCoords() = m_game->inputManager.getMouseCoords();
	m_players[0].getWorldMouseCoords() = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());

	float currentFPS = m_game->getFps();
#else
	float currentFPS = m_tps;
#endif
	// update all players
	for (unsigned long i = 0; i < m_players.size(); i++) {
		if(m_players[i].update(currentFPS, m_boxes, m_chunks, m_world)) {
			updateActiveBodies();
		}
	}

	// Update Box2D physics sim
	m_world->Step(1.0f / currentFPS, 8, 3);
	m_world->ClearForces();
	
	// server 
	if (isServer) {
		processPlayerInput();
		//TODO: Handle player join/leave events
		createUpdateFiles();
	}
}

void GameplayScreen::updateActiveBodies()
{
	// set pPos to player[0]s position
	glm::vec2 pPos;
	pPos.x = round(m_players[0].getPosition().x);
	pPos.y = round(m_players[0].getPosition().y);
	// loop m_chunks and if pos is too far away, deactivate the chunk
	
	for (unsigned long i = 0; i < m_chunks.size(); i++) {
		glm::vec2 chunkPos;
		chunkPos.x = m_chunks[i].m_pos.x;
		chunkPos.y = m_chunks[i].m_pos.y;

		if (abs(chunkPos.x - (pPos.x / 16)) >= 2 || abs(chunkPos.y - (pPos.y / 16)) >= 2) {
			delete m_chunks[i].m_chunk;
		}
	}
	
	// ACTIVATE NEW | get player Pos
	// do some math to loop through Block_S's around the player
	// check if m_hasCounterpart = false && m_blockID != 0, create counterpart	
	for (int i = (int)(pPos.y - 2) / 16; i < (int)(pPos.y + 2) / 16; i++) {
		for (int j = (int)(pPos.x - 2) / 16; j < (int)(pPos.x + 2) / 16; j++) {
	
			bool createChunk = true;
			for (unsigned long k = 0; k < m_chunks.size(); k++) {
				if (m_chunks[k].m_pos == glm::vec2(i,j)) {
					createChunk = false;
				}
			}	
		
			if (createChunk) {
				BLOCK_QUAD q = Helper::getQuad(glm::vec2(i, j));
				chunk_S newChunk;
				switch(q) {
					case BLOCK_QUAD::TL:
						newChunk.m_chunk->setQuad(BLOCK_QUAD::TL);
					break;
					case BLOCK_QUAD::TR:
						newChunk.m_chunk->setQuad(BLOCK_QUAD::TR);
					break;
					case BLOCK_QUAD::BL:
						newChunk.m_chunk->setQuad(BLOCK_QUAD::BL);
					break;
					case BLOCK_QUAD::BR:
						newChunk.m_chunk->setQuad(BLOCK_QUAD::BR);
					break;
					case BLOCK_QUAD::NO_QUAD:
						std::cout << "NO_QUAD!\n\n";
					break;
				}	
				newChunk.m_pos = glm::vec2(abs(i) % 16, abs(j) % 16);
				ReaderWriter::loadChunk(m_world, &m_chunks, &newChunk);
			}
		}
	}
}

#ifdef CLIENT
//-------------------------------------------------------------------
void GameplayScreen::draw()
{
	// Set the base depth to 1.0
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_textureProgram.use();

	// upload texture uniform
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	// camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//-----------------------------------
	m_spriteBatch.begin();

	// draw boxes
	for (auto& b : m_boxes) {
		b.draw(m_spriteBatch);
	}

	// draw blocks
	for (auto& c : m_chunks) {
		c.m_chunk->draw(m_spriteBatch);
	}
	
	// draw player
	m_players[0].draw(m_spriteBatch, m_game);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_textureProgram.unuse();

	// lighting
	Light playerLight;
	playerLight.color = AE::Color_rgb8(255, 165, 0, 42);
	playerLight.position = m_players[0].getPosition();
	playerLight.size = 60.0f;

	m_lightProgram.use();
	pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	// additive blending for light drawing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_spriteBatch.begin();

	playerLight.draw(m_spriteBatch);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_lightProgram.unuse();
	
	// reset to alpha blending for other drawing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw debug lines
	if (m_renderDebug) {
		glm::vec4 destRect;
		
		// draw boxes debug
		for (unsigned long i = 0; i < m_boxes.size(); i++) {
			m_boxes[i].drawDebug(m_debugRenderer);
		}

		// draw blocks debug
		for (auto& c : m_chunks) {
			c.m_chunk->drawDebug(m_debugRenderer);
		}

		// draw player debug
		m_players[0].drawDebug(m_debugRenderer);

		m_debugRenderer.end();
		m_debugRenderer.render(projectionMatrix, 2.0f);
	}
	// draw GUI
	drawHud();
}

//-------------------------------------------------------------------
void GameplayScreen::checkInput()
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
	}

	// increase fps
	if (m_game->inputManager.isKeyJustPressed(SDLK_RIGHT)) {
		m_game->setMaxFPS(m_game->MAX_FPS + 10);
	// decrease fps
	} else if (m_game->inputManager.isKeyJustPressed(SDLK_LEFT)) {
		m_game->setMaxFPS(m_game->MAX_FPS - 10);
	}

	// enable debug mode
	if (m_game->inputManager.isKeyJustPressed(SDLK_F1)) {
		if (!m_renderDebug) {
			m_renderDebug = true;
		} else {
			m_renderDebug = false;
		}
	}
	m_gui.onSDLEvent(evnt);
}

//-------------------------------------------------------------------
void GameplayScreen::drawHud() 
{	
	if (m_renderDebug) {
		m_debugButton->show();
		static int frameCounter = 0;
		static int estimatedFPS = 60;
		frameCounter++;
		// update fps counter every half second
		if (frameCounter >= m_game->getFps() / 2) {
			estimatedFPS = m_game->getFps();
			frameCounter = 0;
			estimatedFPS = round(estimatedFPS);
		}
		m_debugButton->setText("FPS: " + std::to_string(estimatedFPS) + "/" + std::to_string(m_game->MAX_FPS) 
						+ "\n Active Chunks: " + std::to_string(m_chunks.size())
						+ "\n X: " + std::to_string((int)round(m_players[0].getPosition().x))
						+ "   |   Y: " + std::to_string((int)round(m_players[0].getPosition().y)));
	} else {
		m_debugButton->hide();
	}
	m_gui.draw();
	glEnable(GL_BLEND);	
}

//-------------------------------------------------------------------
bool GameplayScreen::onDebugButtonClick(const CEGUI::EventArgs&)
{
	if (m_renderDebug) {
		m_renderDebug = false;
	} else {
		m_renderDebug = true;
    }
	return true;
}
#endif

//-------------------------------------------------------------------
void GameplayScreen::processPlayerInput()
{
	//TODO: Give each player a different id and use that instead of trusting the player to give you the correct id.
	for (unsigned long i = 0; i < m_players.size(); i++) {
		std::ifstream file("players/" + std::to_string(i));
		
		// set player mouse coords
		float tmpX, tmpY;
		file >> tmpX;
		file >> tmpY;
		m_players[i].getScreenMouseCoords() = glm::vec2(tmpX, tmpY);
		// set player buttons down
		unsigned int tmp;
		while(file >> tmp) {
			m_players[i].getPressedKeys().push_back(tmp);
		}
	}
}

//-------------------------------------------------------------------
void GameplayScreen::createUpdateFiles()
{
	//TODO: update only what needs to be updated
	std::ofstream file("updates/player");
}

//-------------------------------------------------------------------
void GameplayScreen::playerJoin()
{
	Player player;
	//TODO: Handle player position, color, texture, etc
	player.init(m_world, 
					glm::vec2(0.0f, 10.0f),
					glm::vec2(2.0f, 2.0f),
					glm::vec2(1.1f, 1.8f),
					AE::Color_rgb8(255, 255, 255, 255));
					//-----------------------------------
	m_players.push_back(player);
}

//-------------------------------------------------------------------
void GameplayScreen::playerLeave(unsigned long playerIndex)
{
	m_players.erase(m_players.begin() + playerIndex);
}

