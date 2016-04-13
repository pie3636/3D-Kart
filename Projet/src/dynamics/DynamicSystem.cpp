#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "./../../include/gl_helper.hpp"
#include "./../../include/dynamics/DynamicSystem.hpp"
#include "./../../include/dynamics/KartPlaneCollision.hpp"
#include "./../../include/dynamics/KartKartCollision.hpp"


DynamicSystem::DynamicSystem() :
    m_dt(0.1),
    m_restitution(1.0),
    m_handleCollisions(true)
{
}


const std::vector<KartPtr>& DynamicSystem::getKarts() const
{
    return m_Karts;
}

void DynamicSystem::setKarts(const std::vector<KartPtr> &Karts)
{
    m_Karts = Karts;
}

const std::vector<ForceFieldPtr>& DynamicSystem::getForceFields() const
{
    return m_forceFields;
}

void DynamicSystem::setForceFields(const std::vector<ForceFieldPtr> &forceFields)
{
    m_forceFields = forceFields;
}


float DynamicSystem::getDt() const
{
    return m_dt;
}

void DynamicSystem::setDt(float dt)
{
    m_dt = dt;
}

DynamicSystem::~DynamicSystem() {}

void DynamicSystem::clear()
{
    m_Karts.clear();
    m_forceFields.clear();
    m_planeObstacles.clear();
}

bool DynamicSystem::getCollisionDetection()
{
    return m_handleCollisions;
}

void DynamicSystem::setCollisionsDetection(bool onOff)
{
    m_handleCollisions = onOff;
}

void DynamicSystem::addKart(KartPtr p)
{
    m_Karts.push_back(p);
}

void DynamicSystem::addForceField(ForceFieldPtr forceField)
{
    m_forceFields.push_back(forceField);
}

void DynamicSystem::addPlaneObstacle(PlanePtr planeObstacle)
{
    m_planeObstacles.push_back(planeObstacle);
}

SolverPtr DynamicSystem::getSolver()
{
    return m_solver;
}

void DynamicSystem::setSolver(SolverPtr solver)
{
    m_solver = solver;
}

void DynamicSystem::detectCollisions()
{
    //Detect Kart plane collisions
    for(KartPtr p : m_Karts)
    {
        for(PlanePtr o : m_planeObstacles)
        {
            if(testKartPlane(p, o))
            {
                KartPlaneCollisionPtr c = std::make_shared<KartPlaneCollision>(p,o,m_restitution);
                m_collisions.push_back(c);
            }
        }
    }

    //Detect Kart Kart collisions
    for(size_t i=0; i<m_Karts.size(); ++i)
    {
        for(size_t j=i; j<m_Karts.size(); ++j)
        {
            KartPtr p1 = m_Karts[i];
            KartPtr p2 = m_Karts[j];
            if(testKartKart(p1,p2))
            {
                KartKartCollisionPtr c = std::make_shared<KartKartCollision>(p1,p2,m_restitution);
                m_collisions.push_back(c);
            }
        }
    }
}

void DynamicSystem::solveCollisions()
{
    while(!m_collisions.empty())
    {
        CollisionPtr collision = m_collisions.back();
        collision->solveCollision();
        m_collisions.pop_back();
    }
}

void DynamicSystem::computeSimulationStep()
{
    //Compute Kart's force
    for(KartPtr p : m_Karts)
    {
        p->setForce(glm::vec3(0.0,0.0,0.0));
    }
    for(ForceFieldPtr f : m_forceFields)
    {
        f->addForce();
    }

    //Integrate position and velocity of Karts
    m_solver->solve(m_dt, m_Karts);

    //Detect and resolve collisions
    if(m_handleCollisions)
    {
        detectCollisions();
        solveCollisions();
    }
}

const float DynamicSystem::getRestitution()
{
    return m_restitution;
}

void DynamicSystem::setRestitution(const float& restitution)
{
    m_restitution = std::max(0.0f,std::min(restitution,1.0f));
}

std::ostream& operator<<(std::ostream& os, const DynamicSystemPtr& system)
{
    std::vector<KartPtr> Karts = system->getKarts();
    os << "Kart number: " << Karts.size() << std::endl;
    for(KartPtr p : Karts)
        os << p << std::endl;
    return os;
}
