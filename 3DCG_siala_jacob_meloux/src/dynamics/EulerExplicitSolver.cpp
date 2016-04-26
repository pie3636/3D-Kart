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
            p->setVelocity( p->getVelocity() + dt * ( 1.0f/p->getMass() ) * p->getForce() );
            p->setPosition( p->getPosition() + dt * p->getVelocity() );
        }
    }
}
