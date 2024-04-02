#pragma once

#include "AE.h"
#include "Window.h"
#include "ScreenList.h"
#include "InputManager.h"
#include "Timing.h"

#include <memory>


namespace AE
{
//-------------------------//
class ScreenList;
//-------------------------//

class IMainGame
{
//-------------------------------------------------------------------
	public:
	int MAX_FPS = 60.0f;
	IMainGame();
	virtual	~IMainGame();

	void run();
	void exitGame();

	virtual void onInit() = 0;
	virtual void addScreens() = 0;
	virtual void onExit() = 0;

	float getFps() const { return m_fps; }
	void onSDLEvent(SDL_Event& evnt);

	void setMaxFPS(int fps);

	InputManager inputManager;
//-------------------------------------------------------------------
	protected:
	virtual void update();
	virtual void draw();
		
	bool init();
	bool initSystems();

	AE::FpsLimiter m_limiter;
		
	std::unique_ptr<ScreenList> m_screenList = nullptr;
	IGameScreen* m_currentScreen = nullptr;
	bool m_isRunning = false;
	float m_fps = MAX_FPS;
	Window m_window;
};
}

