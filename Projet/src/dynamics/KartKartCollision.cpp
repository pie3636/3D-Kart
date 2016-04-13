#include "./../../include/dynamics/KartKartCollision.hpp"
#include <glm/gtx/norm.hpp>

KartKartCollision::~KartKartCollision()
{

}

KartKartCollision::KartKartCollision(KartPtr k1, KartPtr k2, float restitution) :
    Collision(restitution)
{
    kart1 = k1;
    kart1 = k2;
}


void KartKartCollision::do_solveCollision()
{
    //Don't process fixed Karts (Let's assume that the ground plane is fixed)
    if (kart1->isFixed() && kart2->isFixed()) return;

    //Compute interpenetration distance
    float KartKartDist = glm::distance(kart1->getPosition(),kart2->getPosition());

    //TODO
    float interpenetrationDist = kart1->getRadius()+kart2->getRadius()-KartKartDist;

    //Compute Kart-Kart vector
    glm::vec3 k = glm::normalize(kart1->getPosition()-kart2->getPosition());

    //Project each Kart along the Kart-Kart vector with half of the interpenetration distance
    //To be more precise, we ponderate the distance with the mass of the Kart
    glm::vec3 prev_x1 = kart1->getPosition();
    glm::vec3 prev_x2 = kart2->getPosition();
    if(kart1->isFixed())
    {
        kart2->setPosition(prev_x2 - interpenetrationDist*k);
    }
    else if(kart2->isFixed())
    {
        kart1->setPosition(prev_x1 + interpenetrationDist*k);
    }
    else
    {
        float c1 = kart1->getMass()/(kart1->getMass()+kart2->getMass());
        float c2 = kart2->getMass()/(kart1->getMass()+kart2->getMass());
        kart1->setPosition(prev_x1 + c2*interpenetrationDist*k);
        kart2->setPosition(prev_x2 - c1*interpenetrationDist*k);
    }

    //Compute post-collision velocity
    glm::vec3 prev_v1 = kart1->getVelocity();
    glm::vec3 prev_v2 = kart2->getVelocity();
    float proj_v = (1.0f+m_restitution)*glm::dot(k, prev_v1-prev_v2)/(1.0 / kart1->getMass() + 1.0 / kart2->getMass());
    glm::vec3 new_v1 = prev_v1 - proj_v/kart1->getMass()*k;
    glm::vec3 new_v2 = prev_v2 + proj_v/kart2->getMass()*k;
    kart1->setVelocity(new_v1);
    kart2->setVelocity(new_v2);
}


bool testKartKart(const KartPtr &k1, const KartPtr &k2)
{
    if(k1==k2) return false;
    float r = k1->getRadius() + k2->getRadius();
    float c = glm::distance2(k1->getPosition(),k2->getPosition()) - r*r;
    return (c<0.0f) ? true : false;
}
