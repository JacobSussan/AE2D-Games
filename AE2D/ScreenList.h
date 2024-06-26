#pragma once

#include "IGameScreen.h"
#include "IMainGame.h"

#include <vector>

namespace AE
{
	class ScreenList
	{
//-------------------------------------------------------------------
		public:
		ScreenList(IMainGame* game);
		~ScreenList();

		IGameScreen* moveNext();
		IGameScreen* movePrevious();
	
		void setScreen(int nextScreen);
		void addScreen(IGameScreen* newScreen);

		void destroy();

		IGameScreen* getCurrent();

//-------------------------------------------------------------------
		protected:
		IMainGame* m_game  = nullptr;
		std::vector<IGameScreen*> m_screens;
		int m_currentScreenIndex = -1;
	};
}

