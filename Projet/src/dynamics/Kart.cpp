#include "./../../include/dynamics/Kart.hpp"


bool Kart::isFixed() const
{
    return m_isFixed;
}

void Kart::setFixed(bool isFixed)
{
    m_isFixed = isFixed;
}

Kart::Kart(const glm::vec3 &position, const glm::vec3 &velocity, const float &mass,
  const float& width, const float& length, const float& depth, const float& angle)
    : m_initialPosition( position ), m_initialVelocity( velocity ), m_initialAngle(angle),
        m_position(position),
      m_velocity(velocity),
      m_force(glm::vec3(0.0,0.0,0.0)),
      m_mass(mass),
      m_angle(angle		),
      length(length),
      width(width),
      depth(depth)
{}

Kart::~Kart()
{}


const glm::vec3 & Kart::getPosition() const
{
    return m_position;
}

const glm::vec3 & Kart::getVelocity() const
{
    return m_velocity;
}

const glm::vec3 & Kart::getForce() const
{
    return m_force;
}

float Kart::getMass() const
{
    return m_mass;
}

void Kart::setPosition(const glm::vec3 &pos)
{
    m_position = pos;
}

void Kart::setVelocity(const glm::vec3 &vel)
{
    m_velocity = vel;
}

void Kart::setForce(const glm::vec3 &force)
{
    m_force = force;
}

void Kart::incrPosition(const glm::vec3 &pos)
{
    m_position += pos;
}

void Kart::incrVelocity(const glm::vec3 &vel)
{
    m_velocity += vel;
}

void Kart::incrForce(const glm::vec3& force)
{
    m_force +=force;
}

void Kart::restart()
{
  m_position = m_initialPosition;
  m_velocity = m_initialVelocity;
  m_angle	 = m_initialAngle;
}

std::ostream& operator<<(std::ostream& os, const KartPtr& p)
{
    const glm::vec3& x = p->getPosition();
    const glm::vec3& v = p->getVelocity();

    os << "pos (" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    os << " ; ";
    os << "vel (" << v[0] << ", " << v[1] << ", " << v[2] << ")";

    return os;
}
