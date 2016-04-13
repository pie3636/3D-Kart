#include "./../../include/dynamics/DampingForceField.hpp"

DampingForceField::DampingForceField(const std::vector<KartPtr> Karts, const float damping)
{
    m_Karts = Karts;
    m_damping = damping;
}

void DampingForceField::do_addForce()
{
    for(KartPtr p : m_Karts)
    {
        p->incrForce(-m_damping*p->getVelocity());
    }
}

const std::vector<KartPtr> DampingForceField::getKarts()
{
    return m_Karts;
}

void DampingForceField::setKarts(const std::vector<KartPtr>& Karts)
{
    m_Karts = Karts;
}

const float& DampingForceField::getDamping()
{
    return m_damping;
}

void DampingForceField::setDamping(const float& damping)
{
    m_damping = damping;
}
