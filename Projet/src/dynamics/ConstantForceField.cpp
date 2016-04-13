#include "./../../include/dynamics/ConstantForceField.hpp"

ConstantForceField::ConstantForceField(const std::vector<KartPtr>& Karts, const glm::vec3& force)
{
    m_Karts = Karts;
    m_force = force;
}

void ConstantForceField::do_addForce()
{
    for(KartPtr p : m_Karts)
    {
        p->incrForce(m_force*p->getMass());
    }
}

const std::vector<KartPtr> ConstantForceField::getKarts()
{
    return m_Karts;
}

void ConstantForceField::setKarts(const std::vector<KartPtr>& Karts)
{
    m_Karts = Karts;
}

const glm::vec3& ConstantForceField::getForce()
{
    return m_force;
}

void ConstantForceField::setForce(const glm::vec3& force)
{
    m_force = force;
}
