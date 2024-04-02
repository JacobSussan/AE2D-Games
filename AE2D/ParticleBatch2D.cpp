#include "ParticleBatch2D.h"

//-------------------------------------------------------------------
AE::ParticleBatch2D::ParticleBatch2D() 
{

}

//-------------------------------------------------------------------
AE::ParticleBatch2D::~ParticleBatch2D() 
{
	delete[] m_particles;
}

//-------------------------------------------------------------------
void AE::ParticleBatch2D::defaultUpdate(Particle2D& particle, float deltaTime)
{
	particle.pos += particle.velocity * deltaTime;
}

//-------------------------------------------------------------------
void AE::ParticleBatch2D::init(int maxParticles,
									 float decayRate,
									 GLTexture texture,
									 std::function<void(Particle2D&, float)> updateFunc /* = defaultUpdate */) 
{
	m_maxParticles = maxParticles;
	m_particles = new Particle2D[m_maxParticles];
	m_decayRate = decayRate;
	m_texture = texture;
	m_updateFunc = updateFunc;
}

//-------------------------------------------------------------------
void AE::ParticleBatch2D::update(float deltaTime) 
{
	for (int i = 0; i < m_maxParticles; i++) {
		if (m_particles[i].life > 0.0f) {
			m_updateFunc(m_particles[i], deltaTime);
			m_particles[i].life -= m_decayRate * deltaTime;
		}
	}
}

//-------------------------------------------------------------------
void AE::ParticleBatch2D::draw(SpriteBatch* spriteBatch) 
{
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < m_maxParticles; i++) {
		auto& p = m_particles[i];
		if (p.life > 0.0f) {
			glm::vec4 destRect(p.pos.x, p.pos.y, p.width, p.width);
			spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
		}
	}
}

//-------------------------------------------------------------------
void AE::ParticleBatch2D::addParticle(const glm::vec2& position,
											const glm::vec2& velocity,
											const Color_rgb8& color,
											float width) 
{
	int particleIndex = findFreeParticle();
	auto& p = m_particles[particleIndex];
	p.life = 1.0f;
	p.pos = position;
	p.velocity = velocity;
	p.color = color;
	p.width = width;

}

//-------------------------------------------------------------------
int AE::ParticleBatch2D::findFreeParticle() 
{
	for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
		if (m_particles[i].life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}

	for (int i = 0; i < m_lastFreeParticle; i++) {
		if (m_particles[i].life <= 0.0f) {
			m_lastFreeParticle = i;
			return i;
		}
	}
	return 0;
}
