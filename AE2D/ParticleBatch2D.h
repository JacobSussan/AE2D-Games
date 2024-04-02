#pragma once

#include <glm/glm.hpp>
#include <functional>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace AE {

//-------------------------------------------------------------------
class Particle2D 
{
public:
	glm::vec2 pos = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	Color_rgb8 color;
	float life = 0.0f;
	float width = 0.0f;
};

//-------------------------------------------------------------------s
class ParticleBatch2D 
{
public:
	ParticleBatch2D();
	~ParticleBatch2D();

	static void defaultUpdate(Particle2D& particle, float deltaTime);

	void init(int maxParticles,
			  float decayRate,
			  GLTexture texture,
			  std::function<void(Particle2D&, float)> updateFunc = defaultUpdate);
	void update(float deltaTime);
	void draw(SpriteBatch* spriteBatch);
	void addParticle(const glm::vec2& position,
					 const glm::vec2& velocity,
					 const Color_rgb8& color,
					 float width);
private:
	int findFreeParticle();
	std::function<void(Particle2D&, float)> m_updateFunc;

	float m_decayRate = 0.1f;
	Particle2D* m_particles = nullptr;
	int m_maxParticles = 0;
	int m_lastFreeParticle = 0;
	GLTexture m_texture;
};
}

