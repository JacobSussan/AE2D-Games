#include "Capsule.h"

//-------------------------------------------------------------------
void Capsule::init(std::vector<b2Body*> bodies)
//-----------------------------------
{
	m_bodies = bodies;

	// loop bodies and set them to all be inactive
	for (unsigned long i = 0; i < m_bodies.size(); i++) {
		m_bodies[i]->SetActive(0);
	}
}

//-------------------------------------------------------------------
void Capsule::setActiveBody(int& currentBodyIndex, int bodyIndex)
{
	// copy everything from the current active body to the new active body
	m_bodies[bodyIndex]->SetTransform(m_bodies[currentBodyIndex]->GetPosition(), m_bodies[currentBodyIndex]->GetAngle());
	m_bodies[bodyIndex]->SetLinearVelocity(m_bodies[currentBodyIndex]->GetLinearVelocity());
	m_bodies[bodyIndex]->SetAngularVelocity(m_bodies[currentBodyIndex]->GetAngularVelocity());
	m_bodies[bodyIndex]->SetLinearDamping(m_bodies[currentBodyIndex]->GetLinearDamping());
	m_bodies[bodyIndex]->SetAngularDamping(m_bodies[currentBodyIndex]->GetAngularDamping());

	// disabe current active body and enable new active body
	m_bodies[currentBodyIndex]->SetActive(0);
	m_bodies[bodyIndex]->SetActive(1);
	
	// switch old body index to new body index
	currentBodyIndex = bodyIndex;
}

