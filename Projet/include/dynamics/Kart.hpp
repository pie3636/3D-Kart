#ifndef KART_HPP
#define KART_HPP

#include <iostream>
#include <memory>
#include <glm/glm.hpp>


class Kart
{
public:

  Kart(
      const glm::vec3& position,
      const glm::vec3& velocity,
      const float& mass,
      const float& width,
      const float& length
    );

  ~Kart();

  const glm::vec3& getPosition() const;

  const glm::vec3& getVelocity() const;

  const glm::vec3& getForce() const;

  float getMass() const;

  float getRadius() const;

  float getLength() const;

  float getWidth() const;

  bool isFixed() const;

  void setPosition(const glm::vec3 &pos);

  void setVelocity(const glm::vec3 &vel);

  void setForce(const glm::vec3 &force);

  void setFixed(bool isFixed);

  void incrPosition(const glm::vec3 &pos);

  void incrVelocity(const glm::vec3 &vel);

  void incrForce(const glm::vec3 &force);

  void restart();

private:

  const glm::vec3 m_initialPosition;

  const glm::vec3 m_initialVelocity;

  glm::vec3 m_position;

  glm::vec3 m_velocity;

  glm::vec3 m_force;

  float m_radius;

  float width;

  float length;


  float m_mass;

  bool m_isFixed;
};

typedef std::shared_ptr<Kart> KartPtr;

/**
 * \brief output stream operator, as non-member
 */
std::ostream& operator<<(std::ostream& os, const KartPtr& p);

#endif
