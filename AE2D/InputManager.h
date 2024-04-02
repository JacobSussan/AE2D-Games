#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace AE {

//-------------------------------------------------------------------
class InputManager 
{
	public:
		InputManager();
		~InputManager();

		void update();
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		void setMouseCoords(float x, float y);
		bool isKeyDown(unsigned int keyID);
		bool isKeyJustPressed(unsigned int keyID);

		//get
		glm::vec2 getMouseCoords() const { return m_mouseCoords; }

		//-----------------------------------
		std::unordered_map<unsigned int, bool> getKeyMap() {
			return m_keyMap;
		}
		//-----------------------------------
		std::unordered_map<unsigned int, bool> getPrevKeyMap() {
			return m_previousKeyMap;
		}
	private:
		bool wasKeyDown(unsigned int keyID);
	
		std::unordered_map<unsigned int, bool> m_keyMap;
		std::unordered_map<unsigned int, bool> m_previousKeyMap;
		glm::vec2 m_mouseCoords = (glm::vec2)0.0;
	};
}
