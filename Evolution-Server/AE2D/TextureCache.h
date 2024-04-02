#pragma once
#include <map>
#include "GLTexture.h"
namespace AE {

//-------------------------------------------------------------------
class TextureCache //Caches the textures so that multiple sprites can use the same textures
{
public:
    TextureCache();
    ~TextureCache();

    GLTexture getTexture(std::string texturePath);

    float getTextureWidth(std::string texturePath);
    float getTextureHeight(std::string texturePath);
private:
    std::map<std::string, GLTexture> m_textureMap;
};
}
