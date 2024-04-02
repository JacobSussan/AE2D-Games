#pragma once
#include <GL/glew.h>

#include <string>

namespace AE {

//-------------------------------------------------------------------
class GLTexture
{
public:
	int getTextureWidth() { return width; }
	int getTextureHeight() { return height; }

	//-----------------------------------
	std::string filePath = "";
	GLuint id;
    int width;
    int height;
};
}
