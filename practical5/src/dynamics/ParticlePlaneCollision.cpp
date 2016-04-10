#include "./../../include/dynamics/ParticlePlaneCollision.hpp"

ParticlePlaneCollision::~ParticlePlaneCollision()
{}


ParticlePlaneCollision::ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

void ParticlePlaneCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_particle->isFixed()) return;
	m_particle->incrPosition((m_particle->getRadius() - glm::distance(m_particle->getPosition(), m_plane->projectOnPlane(m_particle->getPosition())))* m_plane->normal());
	m_particle->incrVelocity(- (1 + m_restitution) * (glm::dot(m_particle->getVelocity(), m_plane->normal())) * m_plane->normal());
}



bool testParticlePlane(const ParticlePtr &particle, const PlanePtr &plane)
{
	glm::vec3 p2 = plane->projectOnPlane(particle->getPosition());
    return glm::distance(particle->getPosition(), p2) < particle->getRadius();
    
  return false;
}
