#include "acpch.h"
#include "RigidBody2D.h"

namespace ac
{
    RigidBody2D::RigidBody2D()
        : mass(1.0f)
        , inverseMass(1.0f)
        , velocity(0.0f)
        , angularVelocity(0.0f)
        , force(0.0f)
        , torque(0.0f)
        , restitution(0.5f)
        , friction(0.3f)
        , useGravity(true)
        , isKinematic(false)
        , freezeRotation(false)
        , inertiaTensor(1.0f)
    {
    }

    RigidBody2D::RigidBody2D(float _mass, float _restitution, float _friction, bool _useGravity, bool _isKinematic, bool _freezeRotation)
        : mass(_mass)
        , inverseMass(_mass > 0.0f ? 1.0f / _mass : 0.0f)
        , velocity(0.0f)
        , angularVelocity(0.0f)
        , force(0.0f)
        , torque(0.0f)
        , restitution(_restitution)
        , friction(_friction)
        , useGravity(_useGravity)
        , isKinematic(_isKinematic)
        , freezeRotation(_freezeRotation)
        , inertiaTensor(1.0f)
    {
    }
    
    RigidBody2D::RigidBody2D(float _mass)
        : mass(_mass)
        , inverseMass(_mass > 0.0f ? 1.0f / _mass : 0.0f)
        , velocity(0.0f)
        , angularVelocity(0.0f)
        , force(0.0f)
        , torque(0.0f)
        , restitution(0.5f)
        , friction(0.3f)
        , useGravity(true)
        , isKinematic(false)
        , freezeRotation(false)
        , inertiaTensor(1.0f)
    {
    }
    
    void RigidBody2D::ApplyForce(const glm::vec2& _force)
    {
        // Only apply force if not kinematic
        if (isKinematic)
            return;
            
        force += _force;
    }
    
    void RigidBody2D::ApplyForceAtPosition(const glm::vec2& _force, const glm::vec2& _position)
    {
        // Only apply force if not kinematic
        if (isKinematic)
            return;
            
        // Apply the force
        force += _force;
        
        // Calculate torque if rotation isn't frozen
        if (!freezeRotation)
        {
            // Cross product in 2D is a scalar: (a.x * b.y - a.y * b.x)
            float torqueAmount = _position.x * _force.y - _position.y * _force.x;
            torque += torqueAmount;
        }
    }
    
    void RigidBody2D::ApplyImpulse(const glm::vec2& _impulse)
    {
        // Only apply impulse if not kinematic
        if (isKinematic)
            return;
            
        // Impulse = change in momentum = mass * change in velocity
        // So, change in velocity = impulse / mass
        velocity += _impulse * inverseMass;
    }
    
    void RigidBody2D::ApplyImpulseAtPosition(const glm::vec2& impulse, const glm::vec2& position)
    {
        // Only apply impulse if not kinematic
        if (isKinematic)
            return;
        
        // Apply linear impulse (change in velocity)
        velocity += impulse * inverseMass;
        
        // Calculate angular impulse if rotation isn't frozen
        if (!freezeRotation)
        {
            // Calculate torque using 2D cross product (r × F)
            // position is relative to center of mass
            float angularImpulse = position.x * impulse.y - position.y * impulse.x;
            
            // Apply angular impulse
            // For simplicity, using a moment of inertia approximation
            // Actual moment of inertia would depend on the shape
            
            angularVelocity += angularImpulse / inertiaTensor;
        }
    }
}