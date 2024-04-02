#include "ScreenList.h"
#include "IGameScreen.h"

//-------------------------------------------------------------------
AE::ScreenList::ScreenList(IMainGame* game) : m_game(game)
{
	// Empty
}

//-------------------------------------------------------------------
AE::ScreenList::~ScreenList()
{
	destroy();
}

//-------------------------------------------------------------------
AE::IGameScreen* AE::ScreenList::moveNext()
{
	IGameScreen* currentScreen = getCurrent();

	if (currentScreen->getNextScreenIndex() != -1) {
		m_currentScreenIndex = currentScreen->getNextScreenIndex();
	}

	return getCurrent();
}

//-------------------------------------------------------------------
AE::IGameScreen* AE::ScreenList::movePrevious()
{
	IGameScreen* currentScreen = getCurrent();
	if (currentScreen->getPreviousScreenIndex() != -1) {
		m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
	}
	return getCurrent();
}
	
//-------------------------------------------------------------------
void AE::ScreenList::setScreen(int nextScreen)
{
	m_currentScreenIndex = nextScreen;
}

//-------------------------------------------------------------------
void AE::ScreenList::addScreen(IGameScreen* newScreen)
{
	newScreen->m_screenIndex = m_screens.size();
	m_screens.push_back(newScreen);
	newScreen->build();
	newScreen->setParentGame(m_game);
}

//-------------------------------------------------------------------
void AE::ScreenList::destroy()
{
	for (size_t i = 0; i < m_screens.size(); i++) {
		m_screens[i]->destroy();
	}

	m_screens.resize(0);
	m_currentScreenIndex = -1;
}

//-------------------------------------------------------------------
AE::IGameScreen* AE::ScreenList::getCurrent()
{
	if (m_currentScreenIndex == -1) {
		return nullptr;
	}	

	return m_screens[m_currentScreenIndex];
}

