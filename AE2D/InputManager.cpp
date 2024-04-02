#include "InputManager.h"


//-------------------------------------------------------------------
AE::InputManager::InputManager() 
{

}

//-------------------------------------------------------------------
AE::InputManager::~InputManager() 
{

}

//-------------------------------------------------------------------
void AE::InputManager::update() 
{
	for (auto& it : m_keyMap) { 	//copy m_keyMap to m_previousKeyMap
		m_previousKeyMap[it.first] = it.second;
	}
}

//-------------------------------------------------------------------
void AE::InputManager::pressKey(unsigned int keyID) 
{
	m_keyMap[keyID] = true;
}

//-------------------------------------------------------------------
void AE::InputManager::releaseKey(unsigned int keyID) 
{
	m_keyMap[keyID] = false;
}

//-------------------------------------------------------------------
void AE::InputManager::setMouseCoords(float x, float y) 
{
	m_mouseCoords.x = x;
	m_mouseCoords.y = y;
}

//-------------------------------------------------------------------
bool AE::InputManager::isKeyDown(unsigned int keyID) 
{
	auto it = m_keyMap.find(keyID);
	if (it != m_keyMap.end()) {
		return it->second;
	}
	return false;
}

//-------------------------------------------------------------------
bool AE::InputManager::isKeyJustPressed(unsigned int keyID) 
{
	if (isKeyDown(keyID) && !wasKeyDown(keyID)) return true;
	return false;
}

//-------------------------------------------------------------------
bool AE::InputManager::wasKeyDown(unsigned int keyID) 
{
	auto it = m_previousKeyMap.find(keyID);
	if (it != m_previousKeyMap.end()) {
		return it->second;
	}
	return false;
}

