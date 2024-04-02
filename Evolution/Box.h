#pragma once

#include "AE2D/Vertex.h"
#include "AE2D/SpriteBatch.h"
#include "AE2D/GLTexture.h"
#include "AE2D/DebugRenderer.h"

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>

class Box 
{
//-------------------------------------------------------------------
public:
	//-----------------------------------
	void init(
		b2World* world, 
		const glm::vec2& pos, 
		const glm::vec2& dimentions,
		AE::GLTexture texture,
		const AE::Color_rgb8 color,
		bool fixedRotation,
		bool isDynamic,
		float angle = 0.0f,
		glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//-----------------------------------
	void destroy();
	void draw(AE::SpriteBatch& spriteBatch);
	void drawDebug(AE::DebugRenderer &debugRenderer);
	//-----------------------------------
	b2Body* 		getBody()       { return m_body; }
	b2Fixture* 		getFixture()    { return m_fixture; }
	glm::vec2&		getDimentions() { return m_dimensions; }
	glm::vec4&		getUvRect()		{ return m_uvRect; }
	AE::Color_rgb8& getColor() 		{ return m_color; }
	AE::GLTexture&	getTexture()	{ return m_texture; }
	bool& getFixedRotation()		{ return m_fixedRotation; }

	bool isDynamic() {
		return m_body->GetType() == b2_dynamicBody;
	}
//-------------------------------------------------------------------
private:
	bool m_fixedRotation;
	glm::vec4 m_uvRect;
	b2Body* m_body = nullptr;
	b2Fixture* m_fixture = nullptr;
	glm::vec2 m_dimensions;
	AE::Color_rgb8 m_color;
	AE::GLTexture m_texture;
};

