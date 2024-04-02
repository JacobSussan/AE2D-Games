#pragma once

#include <GL/glew.h>
namespace AE {
//-------------------------------------------------------------------
struct Position 
{
    float x;
    float y;
};

//-------------------------------------------------------------------
struct Color_rgb8 
{
	Color_rgb8() : r(0), g(0), b(0), a(0) {}
	Color_rgb8( GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) { }

    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

//-------------------------------------------------------------------
struct UV 
{
    float u;
    float v;
};

//-------------------------------------------------------------------
struct Vertex 
{
    //This is the position struct. When you store a struct or class
    //inside of another struct or class, it is called composition. This is
    //layed out exactly the same in memory as if we had a float position[2],
    //but doing it this way makes more sense.
    Position position;
	AE::Color_rgb8 Color_rgb8;
    UV uv;

	//----------------------
    void setPosition(float x, float y) 
	{
        position.x = x;
        position.y = y;
    }

	//----------------------
    void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
	{
        Color_rgb8.r = r;
        Color_rgb8.g = g;
        Color_rgb8.b = b;
        Color_rgb8.a = a;
    }

	//----------------------
    void setUV(float u, float v) 
	{
        uv.u = u;
        uv.v = v;
    }
};
}
