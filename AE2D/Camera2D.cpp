#include "Camera2D.h"


//-------------------------------------------------------------------
AE::Camera2D::Camera2D() 
{ 

}

//-------------------------------------------------------------------
AE::Camera2D::~Camera2D() 
{

}

//-------------------------------------------------------------------
void AE::Camera2D::init(int screenWidth, int screenHeight) 
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth, 0.0f, (float)m_screenHeight);
}

//-------------------------------------------------------------------
void AE::Camera2D::update() //updates the camera matrix if needed
{
    if (m_needsMatrixUpdate) {     //Only update if our position or scale have changed
        //Camera Translation
        glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
        m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

        //Camera Scale
        glm::vec3 scale(m_scale, m_scale, 0.0f);
        m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

        m_needsMatrixUpdate = false;
    }
}

//-------------------------------------------------------------------
glm::vec2 AE::Camera2D::convertScreenToWorld(glm::vec2 screenCoords) 
{
	//invert y
	screenCoords.y = m_screenHeight - screenCoords.y;
	//make 0,0 center
	screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
	//scale coords
	screenCoords /= m_scale;
	//translate with camera pos
	screenCoords += m_position;

	return screenCoords;
}

//-------------------------------------------------------------------
bool AE::Camera2D::isBoxInView(const glm::vec2& pos, const glm::vec2& dimensions) 
{
	glm::vec2 scaledScreenDimentions = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

	const float MIN_DISTANCE_X = (dimensions.x / 2.0f) + scaledScreenDimentions.x / 2.0f;
	const float MIN_DISTANCE_Y = (dimensions.y / 2.0f) + scaledScreenDimentions.y / 2.0f;

	glm::vec2 centerPos = pos + (dimensions / 2.0f);
	glm::vec2 distVec = centerPos - m_position;

	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0) return true;
	return false;
}



