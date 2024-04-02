#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

//-------------------------------------------------------------------
AE::Sprite::Sprite() 
{

}

//-------------------------------------------------------------------
AE::Sprite::~Sprite() 
{
    if (m_vboID != 0) {
        glDeleteBuffers(1, &m_vboID);
    }
}

//-------------------------------------------------------------------
void AE::Sprite::init(float x, float y, float width, float height, std::string texturePath) 
{
	//Initializes the sprite VBO. x, y, width, and height are
	//in the normalized device coordinate space. so, [-1, 1]
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;

    m_texture = ResourceManager::getTexture(texturePath);

    //Generate the buffer if it hasn't already been generated
    if (m_vboID == 0) {
        glGenBuffers(1, &m_vboID);
    }

    //This array will hold our vertex data.
    //We need 6 vertices, and each vertex has 2
    //floats for X and Y
    Vertex vertexData[6];

    //First Triangle
    vertexData[0].setPosition(x + width, y + height);
    vertexData[0].setUV(1.0f, 1.0f);

    vertexData[1].setPosition(x, y + height);
    vertexData[1].setUV(0.0f, 1.0f);

    vertexData[2].setPosition(x, y);
    vertexData[2].setUV(0.0f, 0.0f);

    //Second Triangle
    vertexData[3].setPosition(x, y);
    vertexData[3].setUV(0.0f, 0.0f);

    vertexData[4].setPosition(x + width, y);
    vertexData[4].setUV(1.0f, 0.0f);

    vertexData[5].setPosition(x + width, y + height);
    vertexData[5].setUV(1.0f, 1.0f);

    //Set all vertex colors to magenta
    for (int i = 0; i < 6; i++) {
        vertexData[4].setColor(255, 0, 255, 255);
    }

    vertexData[4].setColor(0, 0, 255, 255);

    vertexData[4].setColor(0, 255, 0, 255);


    //Tell opengl to bind our vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    //Upload the data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    //Unbind the buffer (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//-------------------------------------------------------------------
void AE::Sprite::draw() //Draws the sprite to the screen
{
    glBindTexture(GL_TEXTURE_2D, m_texture.id);

    //bind the buffer object
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

    //Tell opengl that we want to use the first
    //attribute array. We only need one array right
    //now since we are only using position.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

    //This is the position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    //This is the Color_rgb8 attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Color_rgb8));
    //This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    //Draw the 6 vertices to the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //Disable the vertex attrib array. This is not optional.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

    //Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

