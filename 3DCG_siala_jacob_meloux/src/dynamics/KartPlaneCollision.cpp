#include "./../../include/dynamics/KartPlaneCollision.hpp"

KartPlaneCollision::~KartPlaneCollision()
{}


KartPlaneCollision::KartPlaneCollision(KartPtr Kart, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_Kart = Kart;
    m_plane = plane;
}

void KartPlaneCollision::do_solveCollision()
{
    // Don't process fixed Karts (Let's assume that the ground plane is fixed)
    if (m_Kart->isFixed()) return;
	
	glm::vec3 avGauche(- m_Kart->getLength()/2 * cos(m_Kart->getAngle()), + m_Kart->getWidth()/2 * sin(m_Kart->getAngle()), 0);
	glm::vec3 avDroite(+ m_Kart->getLength()/2 * cos(m_Kart->getAngle()), + m_Kart->getWidth()/2 * sin(m_Kart->getAngle()), 0);
	glm::vec3 arGauche(- m_Kart->getLength()/2 * cos(m_Kart->getAngle()), - m_Kart->getWidth()/2 * sin(m_Kart->getAngle()), 0);
	glm::vec3 arDroite(+ m_Kart->getLength()/2 * cos(m_Kart->getAngle()), - m_Kart->getWidth()/2 * sin(m_Kart->getAngle()), 0);
	
	glm::vec3 projAvG = m_plane->projectOnPlane(avGauche) - avGauche;
	glm::vec3 projAvD = m_plane->projectOnPlane(avDroite) - avDroite;
	glm::vec3 projArG = m_plane->projectOnPlane(arGauche) - arGauche;
	glm::vec3 projArD = m_plane->projectOnPlane(arDroite) - arDroite;
	
	glm::vec3 norm = m_plane->normal();
	
	float factAvG = projAvG.x != 0 && norm.x != 0 ? projAvG.x / norm.x : (projAvG.y != 0 && norm.y != 0 ? projAvG.y / norm.y : 0);
	float factAvD = projAvD.x != 0 && norm.x != 0 ? projAvD.x / norm.x : (projAvD.y != 0 && norm.y != 0 ? projAvD.y / norm.y : 0);
	float factArG = projArG.x != 0 && norm.x != 0 ? projArG.x / norm.x : (projArG.y != 0 && norm.y != 0 ? projArG.y / norm.y : 0);
	float factArD = projArD.x != 0 && norm.x != 0 ? projArD.x / norm.x : (projArD.y != 0 && norm.y != 0 ? projArD.y / norm.y : 0);
	
	if (factAvG * factAvD * factArG * factArD != 0 || factAvG * factAvD * factArG * factArD >= 0 && factAvG * factAvD >= 0 && factAvG * factArG >= 0) { // Horizontal collision
		if (glm::distance(m_Kart->getPosition(), m_plane->projectOnPlane(m_Kart->getPosition())) >= m_Kart->getDepth()/2) { // Vertical collision
			return;
		}
	}
	
	m_Kart->incrVelocity(- (1 + m_restitution) * (glm::dot(m_Kart->getVelocity(), m_plane->normal())) * m_plane->normal());
	if (glm::distance(m_Kart->getPosition(), m_plane->projectOnPlane(m_Kart->getPosition())) <= m_Kart->getDepth()/2) {
		m_Kart->incrPosition((m_Kart->getDepth()/2 - glm::distance(m_Kart->getPosition(), m_plane->projectOnPlane(m_Kart->getPosition())))* m_plane->normal());
	} else {
		m_Kart->incrPosition((m_Kart->getRadius() - glm::distance(m_Kart->getPosition(), m_plane->projectOnPlane(m_Kart->getPosition())))* m_plane->normal()); // Ceci est inexact (hitbox rectangulaire != sphere) mais nous n'avons pas le temps de faire mieux
	}
}



bool testKartPlane(const KartPtr &kart, const PlanePtr &plane)
{
    glm::vec3 p2 = plane->projectOnPlane(kart->getPosition());
    return glm::distance(kart->getPosition(), p2) < kart->getRadius();
}
