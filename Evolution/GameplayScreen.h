#pragma once

#include "Helper.h"
#include "Box.h"
#include "Chunk.h"
#include "Blocks/Air.h"
#include "Blocks/Brick.h"
#include "Blocks/LightBrick.h"
#include "Blocks/Glass.h"
#include "Blocks/RainbowBrick.h"
#include "BlockData.h"
#include "Player.h"

#include <Box2D/Box2D.h>

#include <vector>

#ifdef CLIENT
	#include "AE2D/IGameScreen.h"
	#include "AE2D/SpriteBatch.h"
	#include "AE2D/GLSLProgram.h"
	#include "AE2D/Camera2D.h"
	#include "AE2D/GLTexture.h"
	#include "AE2D/Window.h"
	#include "AE2D/DebugRenderer.h"
	#include "AE2D/GUI.h"
#endif

#ifdef CLIENT
class GameplayScreen : public AE::IGameScreen
#else
class GameplayScreen
#endif
{
//-------------------------------------------------------------------
public:
#ifdef CLIENT
	// client only
	GameplayScreen(AE::Window* window);
	virtual int getNextScreenIndex()     const override;
	virtual int getPreviousScreenIndex() const override;
	virtual void build()   override;
	virtual void destroy() override;
	virtual void onEntry() override;
	virtual void onExit()  override;
	virtual void update()  override;
	virtual void draw()    override;
	void drawHud();
#else 
	// dedicated server only
	GameplayScreen();
	void begin();
	void build();
	void destroy();
	void update(); // non-virtual
#endif
	// both
	void updateActiveBodies();
	void playerJoin();
	void playerLeave(unsigned long playerIndex);
	void processPlayerInput();
	void createUpdateFiles();
	~GameplayScreen();


//-------------------------------------------------------------------
private:
#ifdef CLIENT
	// client only
	void initGUI();
	bool onDebugButtonClick(const CEGUI::EventArgs&);
	void checkInput();
	//-----------------------------------
	// Engine things
	AE::SpriteBatch 	m_spriteBatch;
	AE::GLSLProgram 	m_textureProgram;
	AE::GLSLProgram 	m_lightProgram;
	AE::Camera2D 		m_camera;
	AE::Window		   *m_window;
	AE::DebugRenderer 	m_debugRenderer;
	AE::GUI 			m_gui;
	CEGUI::PushButton  *m_debugButton;		
	//-----------------------------------
	// options
	bool m_renderDebug = false;
#else 
	// dedicated server only
	int MAX_TPS = 60.0f;
	AE::FpsLimiter m_limiter;
	float m_tps = MAX_TPS;
#endif
	//-----------------------------------
	// both
	std::vector<Player> 	 m_players;
	std::vector<Box> 		 m_boxes;		// all boxes
	std::vector<chunk_S> 	 m_chunks;		// active chunks list
	
	b2World* m_world;
};

