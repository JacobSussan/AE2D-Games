#include "ParticleEngine2D.h"

#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

//-------------------------------------------------------------------
AE::ParticleEngine2D::ParticleEngine2D() 
{

}

//-------------------------------------------------------------------
AE::ParticleEngine2D::~ParticleEngine2D() 
{
	for (auto b : m_batches) {
		delete b;
	}
}

//-------------------------------------------------------------------
void AE::ParticleEngine2D::addBatch(ParticleBatch2D* batch) 
{
	m_batches.push_back(batch);
}

//-------------------------------------------------------------------
void AE::ParticleEngine2D::update(float deltaTime) 
{
	for (auto& b : m_batches) {
		b->update(deltaTime);
	}
}

//-------------------------------------------------------------------
void AE::ParticleEngine2D::draw(SpriteBatch* spriteBatch) 
{
	for(auto& b : m_batches){
		spriteBatch->begin();
		b->draw(spriteBatch);
		spriteBatch->end();
		spriteBatch->renderBatch();
	}
}

