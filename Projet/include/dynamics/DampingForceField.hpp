#ifndef DAMPING_FORCE_FIELD_HPP
#define DAMPING_FORCE_FIELD_HPP

#include <vector>
#include "ForceField.hpp"
#include "Kart.hpp"

/**@brief Implement a damping force field.
 *
 * This class implement a force field that models a damping, i.e.
 * a kinetic friction.
 */
class DampingForceField : public ForceField
{
    public:
        /**@brief Build a damping force field.
         *
         * Build a damping force applied to a set of Karts. The force added
         * to a Kart of velocity v is -damping * v.
         * @param Karts Set of Karts influenced by this damping force.
         * @param damping Damping coefficient. */
        DampingForceField(const std::vector<KartPtr> Karts, const float damping);

        /**@brief Access to the Karts influenced by this force field.
         *
         * Get the Karts influenced by this damping force field.
         * @return The set of Karts influenced by this.
         */
        const std::vector<KartPtr> getKarts();

        /**@brief Define the set of Karts influenced by this force field.
         *
         * Define the set of Karts that will be influenced by this damping
         * force force.
         * @param Karts The new set of influenced Karts.
         */
        void setKarts(const std::vector<KartPtr>& Karts);

        /**@brief Access to the damping factor.
         *
         * Get the damping factor of this force field.
         * @return The damping factor of this.
         */
        const float& getDamping();
        /**@brief Set the damping factor of this force field.
         *
         * Define the damping factor of this damping force field.
         * @param damping The new damping factor.
         */
        void setDamping(const float& damping);

    private:
        void do_addForce();
        std::vector<KartPtr> m_Karts;
        float m_damping;
};

typedef std::shared_ptr<DampingForceField> DampingForceFieldPtr;

#endif // SPRING_HPP
