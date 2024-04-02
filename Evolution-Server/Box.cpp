#include <string>

#include "Box.h"

//-------------------------------------------------------------------
void Box::init(
		b2World* world,
		const glm::vec2& pos,
		const glm::vec2& dimentions,
		AE::GLTexture texture,
		const AE::Color_rgb8 color,
		bool fixedRotation,
		bool isDynamic,
		float angle, /* = 0.0f */
		glm::vec4 uvRect /* = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) */)
		//-----------------------------------
{
	m_fixedRotation = fixedRotation;
	m_dimensions = dimentions;
	m_color = color;
	m_texture = texture;
	m_uvRect = uvRect;
	
	//-----------------------------------
	// Make body
	b2BodyDef bodyDef;
	if (isDynamic) {
		bodyDef.type = b2_dynamicBody;
	} else {
		bodyDef.type = b2_staticBody;
	}

	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.angle = angle;
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(dimentions.x / 2.0f, dimentions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_fixture = m_body->CreateFixture(&fixtureDef);
}

