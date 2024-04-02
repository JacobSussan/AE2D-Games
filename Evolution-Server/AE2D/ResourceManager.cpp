#include "ResourceManager.h"

//-------------------------------------------------------------------
AE::TextureCache AE::ResourceManager::m_textureCache;

//-------------------------------------------------------------------
AE::GLTexture AE::ResourceManager::getTexture(std::string texturePath) 
{
    return m_textureCache.getTexture(texturePath);
}


float AE::ResourceManager::getTextureWidth(std::string texturePath)
{
	return m_textureCache.getTextureWidth(texturePath);
}


float AE::ResourceManager::getTextureHeight(std::string texturePath)
{
	return m_textureCache.getTextureHeight(texturePath);
}

