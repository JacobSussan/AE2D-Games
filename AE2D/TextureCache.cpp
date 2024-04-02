#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

//-------------------------------------------------------------------
AE::TextureCache::TextureCache() 
{

}

//-------------------------------------------------------------------
AE::TextureCache::~TextureCache() 
{

}

//-------------------------------------------------------------------
AE::GLTexture AE::TextureCache::getTexture(std::string texturePath) 
{
    auto mit = m_textureMap.find(texturePath);     //lookup the texture and see if its in the map
    
    if (mit == m_textureMap.end()) {    //check if its not in the map
        //Load the texture
        GLTexture newTexture = ImageLoader::loadPNG(texturePath);
        
        //Insert it into the map
        m_textureMap.insert(make_pair(texturePath, newTexture));

        return newTexture;
    }
    return mit->second;
}

float AE::TextureCache::getTextureWidth(std::string texturePath)
{
	auto mit = m_textureMap.find(texturePath);
	
	return mit->second.getTextureWidth();
}

float AE::TextureCache::getTextureHeight(std::string texturePath)
{
	auto mit = m_textureMap.find(texturePath);

	return mit->second.getTextureHeight();
}


