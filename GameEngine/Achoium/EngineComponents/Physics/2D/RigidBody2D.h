#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace ac
{
    /**
     * @brief Physical properties and behavior component for 2D entities.
     * 
     * RigidBody2D provides 2D physics simulation properties including mass,
     * velocity, forces, and physical material properties.
     */
    class RigidBody2D
    {
    public:
        float mass; ///< Mass of the object in kg
        float inverseMass; ///< Cached inverse mass for calculations (1/mass)
        glm::vec2 velocity; ///< Linear velocity (m/s)
        float angularVelocity; ///< Angular velocity (rad/s)
        glm::vec2 force; ///< Accumulated force to apply
        float torque; ///< Accumulated torque to apply
        float restitution; ///< Bounciness factor (0-1)
        float friction; ///< Friction coefficient
        bool useGravity; ///< Whether gravity affects this object
        bool isKinematic; ///< If true, not affected by forces but affects others
        bool freezeRotation; ///< If true, rotation is not simulated
        float inertiaTensor;
        
        /**
         * @brief Default constructor with standard physics values.
         */
        RigidBody2D();

        /**
         * @brief Constructor with specific properties.
         */
        RigidBody2D(float mass, float restitution, float friction, bool useGravity, bool isKnematic, bool freezeRotation);
        
        /**
         * @brief Constructor with specific mass value.
         * 
         * @param _mass The mass in kg
         */
        explicit RigidBody2D(float _mass);
        
        /**
         * @brief Applies a force to the center of mass.
         * 
         * @param _force Force vector to apply
         */
        void ApplyForce(const glm::vec2& _force);
        
        /**
         * @brief Applies force at a specific position, creating torque.
         * 
         * @param _force Force vector to apply
         * @param _position World position to apply the force at (relative to center)
         */
        void ApplyForceAtPosition(const glm::vec2& _force, const glm::vec2& _position);
        
        /**
         * @brief Applies an instantaneous impulse to velocity.
         * 
         * @param _impulse Impulse vector to apply
         */
        void ApplyImpulse(const glm::vec2& _impulse);

        /**
         * @brief Applies an instantaneous impulse at a specific position, creating torque.
         * 
         * @param impulse Impulse vector to apply
         * @param position World position to apply the impulse at (relative to center)
         */
        void ApplyImpulseAtPosition(const glm::vec2& impulse, const glm::vec2& position);
    };
}