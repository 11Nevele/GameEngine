#include "acpch.h"
#include "RigidBody.h"

namespace ac
{
    RigidBody::RigidBody() : 
        mass(1.0f),
        inverseMass(1.0f),
        velocity(0.0f),
        angularVelocity(0.0f),
        force(0.0f),
        torque(0.0f),
        restitution(0.3f),
        friction(0.5f),
        useGravity(true),
        isKinematic(false),
        freezeRotation(false)
    {
    }

    RigidBody::RigidBody(float mass, float restitution, float friction, bool useGravity, bool isKnematic, bool freezeRotation):
        mass(mass),
        inverseMass(1.0f / mass),
        velocity(0.0f),
        angularVelocity(0.0f),
        force(0.0f),
        torque(0.0f),
        restitution(restitution),
        friction(friction),
        useGravity(useGravity),
        isKinematic(isKnematic),
        freezeRotation(freezeRotation)
    {
    }

    RigidBody::RigidBody(float _mass) :
        velocity(0.0f),
        angularVelocity(0.0f),
        force(0.0f),
        torque(0.0f),
        restitution(0.3f),
        friction(0.5f),
        useGravity(true),
        isKinematic(false),
        freezeRotation(false)
    {
        mass = _mass;
        inverseMass = _mass > 0.0f ? 1.0f / _mass : 0.0f;
    }

    void RigidBody::ApplyForce(const glm::vec3& _force)
    {
        if (!isKinematic)
            force += _force;
    }

    void RigidBody::ApplyForceAtPosition(const glm::vec3& _force, const glm::vec3& _position)
    {
        if (!isKinematic)
        {
            // Add linear force
            force += _force;

            // Compute torque = r × F
            if (!freezeRotation)
                torque += glm::cross(_position, _force);
        }
    }

    void RigidBody::ApplyImpulse(const glm::vec3& _impulse)
    {
        if (!isKinematic)
            velocity += _impulse * inverseMass;
    }
}