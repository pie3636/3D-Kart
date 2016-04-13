#ifndef CONSTANT_FORCE_FIELD_HPP
#define CONSTANT_FORCE_FIELD_HPP

#include <vector>
#include "ForceField.hpp"
#include "Kart.hpp"

/**@brief Constant force field.
 *
 * Implementation of a force field that is constant, i.e. the same for
 * all managed Karts.
 */
class ConstantForceField : public ForceField
{
    public:
        /**@brief Build a constant force field.
         *
         * Build a constant force field for a set of Karts.
         * @param Karts The set of Karts influenced by this force field.
         * @param force The constant force applied to all Karts.
         */
        ConstantForceField(const std::vector<KartPtr>& Karts, const glm::vec3& force);

        /**@brief Access to the set of managed Karts.
         *
         * Get the set of managed Karts of this constant force field.
         * @return The managed force field.
         */
        const std::vector<KartPtr> getKarts();

        /**@brief Define a new set of Karts managed by this constant force field.
         *
         * Set the Karts influenced by this constant force field.
         * @param Karts The new set of Karts.
         */
        void setKarts(const std::vector<KartPtr>& Karts);

        /**@brief Access to the force applied to all influenced Karts.
         *
         * Get the constant force of this force field.
         * @return The force of this force field.
         */
        const glm::vec3& getForce();

        /**@brief Set the force applied to all influenced Karts.
         *
         * Set the force applied to all Karts influenced by this force field.
         * @param force The new force.
         */
        void setForce(const glm::vec3& force);

    private:
        void do_addForce();
        std::vector<KartPtr> m_Karts;
        glm::vec3 m_force;
};

typedef std::shared_ptr<ConstantForceField> ConstantForceFieldPtr;

#endif // SPRING_HPP
