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
    //Don't process fixed Karts (Let's assume that the ground plane is fixed)
    if (m_Kart->isFixed()) return;

    float lx, ly;

    glm::vec3 prev_x = m_Kart->getPosition();
    float distToPlane = glm::dot(prev_x, m_plane->normal())-m_plane->distanceToOrigin();
    //Project the Kart on the plane
    glm::vec3 new_x = prev_x - (distToPlane-m_Kart->getRadius())*m_plane->normal();
    m_Kart->setPosition(new_x);

    //Compute post-collision velocity
    glm::vec3 prev_v = m_Kart->getVelocity();
    glm::vec3 new_v = prev_v - (1.0f + m_restitution)*glm::dot(prev_v, m_plane->normal())*m_plane->normal();
    m_Kart->setVelocity(new_v);
}



bool testKartPlane(const KartPtr &kart, const PlanePtr &plane)
{
    glm::vec3 planePoint = plane->distanceToOrigin()*plane->normal();
    glm::vec3 v = kart->getPosition()-planePoint;
    float kartPlaneDist = glm::dot(v,plane->normal());
    float c = kartPlaneDist-kart->getRadius();
    return (c<0.0f) ? true : false;
    return false;
}
