#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AE {

class Camera2D
{
//-------------------------------------------------------------------
public:
    Camera2D();
    ~Camera2D();

//-----------------------------------
    void init(int screenWidth, int screenHeight);
    void update();
    glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);
    bool isBoxInView(const glm::vec2& pos, 
					const glm::vec2& dimensions);

//-----------------------------------
	void setPosition(const glm::vec2& newPosition)
	{
		m_position = newPosition; m_needsMatrixUpdate = true;
	}

//-----------------------------------
    void setScale(float newScale)
	{
		m_scale = newScale; m_needsMatrixUpdate = true;
	}

//-----------------------------------
    glm::vec2 getPosition()     { return m_position; }
    float     getScale()        { return m_scale; }
    glm::mat4 getCameraMatrix() { return m_cameraMatrix; }

//-------------------------------------------------------------------
private:
	int m_screenWidth = 1920;
	int m_screenHeight = 1080;
    bool m_needsMatrixUpdate = true;
    float m_scale = 1.0f;
    glm::vec2 m_position = (glm::vec2)0.0f;
    glm::mat4 m_cameraMatrix = (glm::mat4)1.0f;
    glm::mat4 m_orthoMatrix = (glm::mat4)1.0f;
};
}
