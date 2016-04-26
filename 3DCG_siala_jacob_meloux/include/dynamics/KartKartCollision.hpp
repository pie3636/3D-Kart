#ifndef KART_KART_COLLISION_HPP
#define KART_KART_COLLISION_HPP

#include "Collision.hpp"
#include "Kart.hpp"


class KartKartCollision : public Collision
{
public:
    ~KartKartCollision();

    KartKartCollision(KartPtr Kart1, KartPtr Kart2, float restitution);

private:
    void do_solveCollision();
    KartPtr kart1;
    KartPtr kart2;
};

typedef std::shared_ptr<KartKartCollision> KartKartCollisionPtr;

bool testKartKart(const KartPtr& p1, const KartPtr& p2);

#endif //Kart_Kart_COLLISION_HPP
