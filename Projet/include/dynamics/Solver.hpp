#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <memory>
#include <vector>
#include "Kart.hpp"

/**@brief Dynamic system solver interface.
 *
 * Define an interface for dynamic system solver.
 */
class Solver
{
public:
  Solver(){}
  virtual  ~Solver(){}
  /**@brief Solve the dynamic system of Karts.
   *
   * Solve the dynamic system of Karts for a specified time step.
   * @param dt The time step for the integration.
   * @param Karts The collection of Karts.
   */
  void solve( const float& dt, std::vector<KartPtr>& Karts );
private:
  /**@brief Solve implementation.
   *
   * The actual implementation to solve the dynamic system. This should
   * be implemented in derived classes.
   * @param dt The time step for the integration.
   * @param Karts The collection of Karts.
   */
  virtual void do_solve(const float& dt, std::vector<KartPtr>& Karts) = 0;
};

typedef std::shared_ptr<Solver> SolverPtr;

#endif //SOLVER_HPP
