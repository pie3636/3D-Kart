# include "../../include/dynamics/Solver.hpp"

void Solver::solve( const float& dt, std::vector<KartPtr>& Karts )
{
  do_solve( dt, Karts );
}
