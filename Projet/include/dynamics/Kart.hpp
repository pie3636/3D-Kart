#ifndef KART_HPP
#define KART_HPP

#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include "../../include/CylinderRenderable.hpp"

class Kart
{
public:

  Kart(
      const glm::vec3& position,
      const glm::vec3& velocity,
      const float& mass,
      const float& width,
      const float& length,
      const float& depth,
      const float& angle,
      CylinderRenderablePtr character = nullptr
    );

  ~Kart();

  const glm::vec3& getPosition() const;
  const glm::vec3& getVelocity() const;
  const glm::vec3& getForce() const;

  float getMass() const;
  float getLength() const {return m_length;};
  float getWidth() const {return m_width;};
  float getDepth() const {return m_depth;};
  float getRadius() const {return m_radius;};
  float getAngle() const {return m_angle;};
  CylinderRenderablePtr getCharacter() const {return m_character;};

  bool isFixed() const;

  void setPosition(const glm::vec3 &pos);
  void setVelocity(const glm::vec3 &vel);
  void setForce(const glm::vec3 &force);
  void setFixed(bool isFixed);
  void setAngle(const float &angle);
  void setCharacter(const CylinderRenderablePtr &character) {this->m_character = character;};

  void incrPosition(const glm::vec3 &pos);
  void incrVelocity(const glm::vec3 &vel);
  void incrForce(const glm::vec3 &force);

  void restart();

private:

  const glm::vec3 m_initialPosition;
  const glm::vec3 m_initialVelocity;
  const float m_initialAngle;

  glm::vec3 m_position;
  glm::vec3 m_velocity;
  glm::vec3 m_force;
  float m_radius;

  float m_width;
  float m_length;
  float m_depth;
  float m_mass;
  float m_angle;

  bool m_isFixed;
  
  CylinderRenderablePtr m_character;
};

typedef std::shared_ptr<Kart> KartPtr;

/**
 * \brief output stream operator, as non-member
 */
std::ostream& operator<<(std::ostream& os, const KartPtr& p);

#endif
