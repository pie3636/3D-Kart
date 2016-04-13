#include "./../../include/dynamics/EulerExplicitSolver.hpp"

EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<KartPtr>& Karts)
{
    for(KartPtr p : Karts)
    {
        if(!p->isFixed())
        {
            //TODO: Implement explicit euler solver
            //Functions to use:
            //Kart::getPosition(), Kart::getVelocity(), Kart::getMass(), Kart::getForce()
            //Kart::setPosition(), Kart::setVelocity()
            p->setVelocity( p->getVelocity() + dt * ( 1.0f/p->getMass() ) * p->getForce() );
            p->setPosition( p->getPosition() + dt * p->getVelocity() );
        }
    }
}
