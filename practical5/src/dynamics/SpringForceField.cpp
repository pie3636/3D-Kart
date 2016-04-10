#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2, float stiffness, float equilibriumLength, float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness),
    m_equilibriumLength(equilibriumLength),
    m_damping(damping)
{}

void SpringForceField::do_addForce()
{
	glm::vec3 u = m_p2->getPosition() - m_p1->getPosition();
	if (glm::length(u) < std::numeric_limits<float>::epsilon()) {
		return;
	}
	glm::vec3 nu = glm::normalize(u);
    glm::vec3 force = -m_stiffness * (glm::length(u) - m_equilibriumLength) * u / glm::length(u) - m_damping * glm::dot(m_p2->getVelocity() - m_p1->getVelocity(), nu) * nu;
	m_p2->incrForce(force);
	m_p1->incrForce(-force);
}

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
