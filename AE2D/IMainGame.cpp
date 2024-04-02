#include "IMainGame.h"

#include <iostream>

//-------------------------------------------------------------------
AE::IMainGame::IMainGame()
{
	m_screenList = std::make_unique<ScreenList>(this);
}

//-------------------------------------------------------------------
AE::IMainGame::~IMainGame() { }
//-------------------------------------------------------------------
void AE::IMainGame::run()
{
	if(!init()) return;

	m_limiter.setMaxFPS(MAX_FPS);

	m_isRunning = true;

	while (m_isRunning) {
		// GameLoop
		m_limiter.begin();

		inputManager.update();
		update();

		if (m_isRunning) {
			draw();	

			m_fps = m_limiter.end();
			m_window.swapBuffer();
		}
	}
}

//-------------------------------------------------------------------
void AE::IMainGame::exitGame()
{
	m_currentScreen->onExit();

	if (m_screenList) {
		m_screenList->destroy();
		m_screenList.reset();
	}
	m_isRunning = false;
}

//-------------------------------------------------------------------
void AE::IMainGame::onSDLEvent(SDL_Event& evnt)
{
	switch (evnt.type) {
		case SDL_QUIT:
			exitGame();
			break;
        case SDL_MOUSEMOTION:
            inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
            break;
        case SDL_KEYDOWN:
        	inputManager.pressKey(evnt.key.keysym.sym);
            break;
        case SDL_KEYUP:
            inputManager.releaseKey(evnt.key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            inputManager.pressKey(evnt.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            inputManager.releaseKey(evnt.button.button);
            break;
        }
}

//-------------------------------------------------------------------
void AE::IMainGame::setMaxFPS(int fps)
{
	MAX_FPS = fps;	
	m_limiter.setMaxFPS(MAX_FPS);
}

//-------------------------------------------------------------------
bool AE::IMainGame::init()
{
	AE::init();

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	if(!initSystems()) return false;

	onInit();
	addScreens();

	m_currentScreen = m_screenList->getCurrent();
	m_currentScreen->onEntry();
	m_currentScreen->setRunning();

	return true;
}

//-------------------------------------------------------------------
bool AE::IMainGame::initSystems()
{
	m_window.create("Evolution", 1920, 1080, 0);
	
	return true;
}

//-------------------------------------------------------------------
void AE::IMainGame::update()
{
	if (m_currentScreen) {
		switch (m_currentScreen->getState()) {
			//-----------------------------------
			case AE::ScreenState::RUNNING:
				m_currentScreen->update();
				break;
			//-----------------------------------
			case AE::ScreenState::CHANGE_NEXT:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->moveNext();
			
				if (m_currentScreen) { 
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
				break;
			//-----------------------------------
			case AE::ScreenState::CHANGE_PREVIOUS:
				m_currentScreen->onExit();
				m_currentScreen = m_screenList->movePrevious();
			
				if (m_currentScreen) { 
					m_currentScreen->setRunning();
					m_currentScreen->onEntry();
				}
			
				break;
			//-----------------------------------
			case AE::ScreenState::EXIT_APP:
				exitGame();
				break;
			//-----------------------------------
			default:
				// Empty
				break;
		}
	} else {
		exitGame();
	}	
}

//-------------------------------------------------------------------
void AE::IMainGame::draw()
{
	glViewport(0,0, m_window.getScreenWidth(), m_window.getScreenHeighth());

	if (m_currentScreen && m_currentScreen->getState() == AE::ScreenState::RUNNING) {
		m_currentScreen->draw();
	}	
}


