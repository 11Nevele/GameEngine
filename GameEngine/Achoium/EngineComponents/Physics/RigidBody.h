#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace ac
{
    /**
     * @brief Physical properties and behavior component for entities.
     * 
     * RigidBody provides physics simulation properties including mass,
     * velocity, forces, and physical material properties.
     */
    class RigidBody
    {
    public:
        float mass; ///< Mass of the object in kg
        float inverseMass; ///< Cached inverse mass for calculations (1/mass)
        glm::vec3 velocity; ///< Linear velocity (m/s)
        glm::vec3 angularVelocity; ///< Angular velocity (rad/s)
        glm::vec3 force; ///< Accumulated force to apply
        glm::vec3 torque; ///< Accumulated torque to apply
        float restitution; ///< Bounciness factor (0-1)
        float friction; ///< Friction coefficient
        bool useGravity; ///< Whether gravity affects this object
        bool isKinematic; ///< If true, not affected by forces but affects others
        bool freezeRotation; ///< If true, rotation is not simulated
        
        /**
         * @brief Default constructor with standard physics values.
         */
        RigidBody();

        RigidBody(float mass, float restitution, float friction, bool useGravity, bool isKnematic, bool freezeRotation);
        
        /**
         * @brief Constructor with specific mass value.
         * 
         * @param _mass The mass in kg
         */
        explicit RigidBody(float _mass);
        
        /**
         * @brief Applies a force to the center of mass.
         * 
         * @param _force Force vector to apply
         */
        void ApplyForce(const glm::vec3& _force);
        
        /**
         * @brief Applies force at a specific position, creating torque.
         * 
         * @param _force Force vector to apply
         * @param _position World position to apply the force at
         */
        void ApplyForceAtPosition(const glm::vec3& _force, const glm::vec3& _position);
        
        /**
         * @brief Applies an instantaneous impulse to velocity.
         * 
         * @param _impulse Impulse vector to apply
         */
        void ApplyImpulse(const glm::vec3& _impulse);
    };
}