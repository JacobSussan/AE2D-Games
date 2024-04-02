#pragma once
#include "TextureCache.h"
#include <string>

//This is a way for us to access all our resources, such as
//Models or textures.
namespace AE {

//-------------------------------------------------------------------
class ResourceManager 
{
public:
    static GLTexture getTexture(std::string texturePath);

    static float getTextureWidth(std::string texturePath);
    static float getTextureHeight(std::string texturePath);

private:
    static TextureCache m_textureCache;
};
}
