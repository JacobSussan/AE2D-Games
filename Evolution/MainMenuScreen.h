#pragma once 

#include "AE2D/GLSLProgram.h"
#include "AE2D/IGameScreen.h"
#include "AE2D/IMainGame.h"
#include "AE2D/Window.h"
#include "AE2D/Camera2D.h"
#include "AE2D/GUI.h"


class MainMenuScreen : public AE::IGameScreen
{
//-------------------------------------------------------------------
public:
	MainMenuScreen(AE::Window* window);
	~MainMenuScreen();

	virtual int getNextScreenIndex()     const override;
	virtual int getPreviousScreenIndex() const override;
		
	virtual void build()   override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit()  override;
	virtual void update()  override;
	virtual void draw()    override;
	void drawHud();
//-------------------------------------------------------------------
private:
	void initGUI();
	void checkInput();

	bool onNewButtonClick(const CEGUI::EventArgs&);
	bool onExitButtonClick(const CEGUI::EventArgs&);

	AE::Camera2D m_camera;
	AE::Window* m_window;
	AE::GUI m_gui;

};


