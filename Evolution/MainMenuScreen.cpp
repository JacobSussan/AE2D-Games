#include "MainMenuScreen.h"	

#include "ScreenIndices.h"

//-------------------------------------------------------------------
MainMenuScreen::MainMenuScreen(AE::Window* window) : m_window(window) 
{
	m_screenIndex = SCREEN_INDEX_MAIN_MENU;
}

//-------------------------------------------------------------------
MainMenuScreen::~MainMenuScreen()
{

}

//-------------------------------------------------------------------
int MainMenuScreen::getNextScreenIndex() const 
{
	return SCREEN_INDEX_GAMEPLAY;
}

//-------------------------------------------------------------------
int MainMenuScreen::getPreviousScreenIndex() const 
{
	return SCREEN_INDEX_NO_SCREEN;
}
		
//-------------------------------------------------------------------
void MainMenuScreen::build() 
{

}

//-------------------------------------------------------------------
void MainMenuScreen::destroy() 
{

}

//-------------------------------------------------------------------
void MainMenuScreen::onEntry() 
{
	m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeighth());
	m_camera.setScale(32.0f);
	initGUI();
}

//-------------------------------------------------------------------
void MainMenuScreen::onExit() 
{
	m_gui.destroy();
}

//-------------------------------------------------------------------
void MainMenuScreen::update() 
{
	m_camera.update();
	checkInput();
}

//-------------------------------------------------------------------
void MainMenuScreen::draw() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
	m_gui.draw();
}

//-------------------------------------------------------------------
void MainMenuScreen::initGUI() 
{
	/// init UI
	m_gui.init("GUI");
	m_gui.loadScheme("TaharezLook.scheme");
	m_gui.loadScheme("VanillaSkin.scheme");
	m_gui.setFont("DejaVuSans-10");

	CEGUI::PushButton* startButton = static_cast<CEGUI::PushButton*>
			(m_gui.createWidget("Vanilla/Button", glm::vec4(0.45f, 0.4f, 0.1f, 0.05f), glm::vec4(0.0f), "NewGameButton"));
	CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>
			(m_gui.createWidget("Vanilla/Button", glm::vec4(0.45f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "ExitButton"));
	
	startButton->setText("New Game");
	exitButton->setText("Exit Game");
	// set event to be called on click
	startButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
							CEGUI::Event::Subscriber(&MainMenuScreen::onNewButtonClick, this));
	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, 
							CEGUI::Event::Subscriber(&MainMenuScreen::onExitButtonClick, this));
}

//-------------------------------------------------------------------
void MainMenuScreen::checkInput() 
{
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		m_game->onSDLEvent(evnt);
		m_gui.onSDLEvent(evnt);
	}
}

//-------------------------------------------------------------------
bool MainMenuScreen::onNewButtonClick(const CEGUI::EventArgs&) 
{
	m_currentState = AE::ScreenState::CHANGE_NEXT;
	return true;
}

//-------------------------------------------------------------------
bool MainMenuScreen::onExitButtonClick(const CEGUI::EventArgs&) 
{
	m_currentState = AE::ScreenState::EXIT_APP;
	return true;
}



