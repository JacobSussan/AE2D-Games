#include "GUI.h"

#include "Helper.h"
#include <SDL2/SDL_timer.h>

CEGUI::OpenGL3Renderer* AE::GUI::m_renderer = nullptr;
		

//-------------------------------------------------------------------
void AE::GUI::init(const std::string& resourceDir)
{
	// check if renderer and system were already init
	if (m_renderer == nullptr) {
		m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();

		//-----------------------------------
		CEGUI::DefaultResourceProvider* rp = 
			static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
		//-----------------------------------
		
		rp->setResourceGroupDirectory("imagesets",	 resourceDir + "/imagesets/");	
		rp->setResourceGroupDirectory("schemes",	 resourceDir + "/schemes/");	
		rp->setResourceGroupDirectory("fonts",		 resourceDir + "/fonts/");	
		rp->setResourceGroupDirectory("layouts",	 resourceDir + "/layouts/");	
		rp->setResourceGroupDirectory("looknfeel", resourceDir + "/looknfeel/");	
		rp->setResourceGroupDirectory("lua_scripts", resourceDir + "/lua_scripts/");
		//-----------------------------------
		CEGUI::ImageManager::setImagesetDefaultResourceGroup ("imagesets");
		CEGUI::Scheme::setDefaultResourceGroup				 ("schemes");
		CEGUI::Font::setDefaultResourceGroup				 ("fonts");
		CEGUI::WidgetLookManager::setDefaultResourceGroup	 ("looknfeel");
		CEGUI::WindowManager::setDefaultResourceGroup		 ("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup		 ("lua_scripts");
	}

	// create context
	m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
	// create and set root
	m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
	m_context->setRootWindow(m_root);
}

//-------------------------------------------------------------------
void AE::GUI::destroy()
{
	CEGUI::System::getSingleton().destroyGUIContext(*m_context);
}

//-------------------------------------------------------------------
void AE::GUI::draw() 
{
	m_renderer->beginRendering();
	m_context->draw();
	m_renderer->endRendering();
	
	glDisable(GL_SCISSOR_TEST);
}

//-------------------------------------------------------------------
void AE::GUI::update()
{
	unsigned int elapsed;
	if (m_lastTime == 0) {
		elapsed = 0;
		m_lastTime = SDL_GetTicks();
	} else {
		unsigned int nextTime = SDL_GetTicks();
		elapsed = nextTime - m_lastTime;
		m_lastTime = nextTime;
	}
	m_context->injectTimePulse((float)elapsed / 1000.0f);
}

//-------------------------------------------------------------------
void AE::GUI::setMouseCursor(const std::string& imageFile)
{
	m_context->getMouseCursor().setDefaultImage(imageFile);
}

//-------------------------------------------------------------------
void AE::GUI::showMouseCursor(bool visiable)
{
	if (visiable) {
		m_context->getMouseCursor().show();
	} else {
		m_context->getMouseCursor().hide();
	}
}

//-------------------------------------------------------------------
void AE::GUI::onSDLEvent(SDL_Event& evnt)
{
	CEGUI::String cs;
	switch (evnt.type) {
		case SDL_MOUSEMOTION:
			m_context->injectMousePosition(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_context->injectKeyDown(AE::Helper::SDLKeyToCEGUIKey(evnt.key.keysym.sym));
			break;
		case SDL_KEYUP:
			m_context->injectKeyUp(AE::Helper::SDLKeyToCEGUIKey(evnt.key.keysym.sym));
			break;
		case SDL_TEXTINPUT:
			cs = CEGUI::String((CEGUI::utf8*) evnt.text.text);
			for(unsigned long i = 0; i < cs.size(); ++i){
				m_context->injectChar(cs[i]);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_context->injectMouseButtonDown(AE::Helper::SDLButtonToCEGUIButton(evnt.button.button));
			break;
		case SDL_MOUSEBUTTONUP:
			m_context->injectMouseButtonUp(AE::Helper::SDLButtonToCEGUIButton(evnt.button.button));
			break;
	}
}

//-------------------------------------------------------------------
void AE::GUI::loadScheme(const std::string& schemeFile) 
{
	CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
}

//-------------------------------------------------------------------
void AE::GUI::setFont(const std::string &fontFile)
{
	CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
	m_context->setDefaultFont(fontFile);
}

//-------------------------------------------------------------------
CEGUI::Window* AE::GUI::createWidget(const std::string& type, 
									 const glm::vec4 destRectPerc,
									 const glm::vec4 destRectPix,
									 const std::string& name /*= ""*/)
								//-----------------------------------
{
	CEGUI::Window *newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
	m_root->addChild(newWindow);

	setWidgetDestRect(newWindow, destRectPerc, destRectPix);

	return newWindow;
}

//-------------------------------------------------------------------
void AE::GUI::setWidgetDestRect(CEGUI::Window *widget, 
								const glm::vec4 destRectPerc,
								const glm::vec4 destRectPix)
								//-----------------------------------
{
	//-----------------------------------
	widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPerc.x, destRectPix.x), 
										CEGUI::UDim(destRectPerc.y, destRectPix.y)));
	//-----------------------------------
	widget->setSize(CEGUI::USize(		CEGUI::UDim(destRectPerc.z, destRectPix.z), 
										CEGUI::UDim(destRectPerc.w, destRectPix.w)));
}


