#pragma once

#include <vector>

namespace AE {

//-------------------------------------------------------------------
class ParticleBatch2D;
class SpriteBatch;

//-------------------------------------------------------------------
class ParticleEngine2D 
{
public:
	ParticleEngine2D();
	~ParticleEngine2D();

	void addBatch(ParticleBatch2D* batch);
	void update(float deltaTime);
	void draw(SpriteBatch* spriteBatch);
private:
	std::vector<ParticleBatch2D*> m_batches;
};
}
