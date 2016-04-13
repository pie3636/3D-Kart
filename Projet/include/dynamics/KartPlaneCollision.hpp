#ifndef KART_PLANE_COLLISION_HPP
#define KART_PLANE_COLLISION_HPP

#include "Collision.hpp"
#include "Kart.hpp"
#include "./../../include/Plane.hpp"

class KartPlaneCollision : public Collision
{
public:
    ~KartPlaneCollision();
    KartPlaneCollision(KartPtr Kart, PlanePtr plane, float restitution);
private:

    void do_solveCollision();
    KartPtr m_Kart;
    PlanePtr m_plane;
};

typedef std::shared_ptr<KartPlaneCollision> KartPlaneCollisionPtr;

bool testKartPlane(const KartPtr& Kart, const PlanePtr& plane);

#endif //Kart_PLANE_COLLISION_HPP
