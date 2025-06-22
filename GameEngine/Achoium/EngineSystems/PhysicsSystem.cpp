#include "acpch.h"
#include "PhysicsSystem.h"
#include "EngineComponents/Physics/Physics.h"
#include "Math/Transform.h"
#include "Event/Event.hpp"
#include "EngineComponents/Time.h"
namespace ac
{
    constexpr glm::vec3 gravity(0.0f, -9.81f, 0.0f);
    
    void PhysicsSystem::PhysicsStep(World& world)
    {
        Time& time = world.GetResourse<Time>();

        // Update physics for all rigidbodies
        world.View<RigidBody, Transform>().ForEach([&time](Entity entity, RigidBody& rb, Transform& transform)
        {
            // Skip kinematic bodies for force calculations
            if (rb.isKinematic)
                return;

            // Apply gravity
            if (rb.useGravity)
                rb.force += gravity * rb.mass;

            // Update velocity with forces (F = ma, so a = F/m)
            rb.velocity += rb.force * rb.inverseMass * time.Delta();
            
            // Update angular velocity with torques
            if (!rb.freezeRotation)
                rb.angularVelocity += rb.torque * rb.inverseMass * time.Delta();

            // Apply linear velocity to position
            transform.position += rb.velocity * time.Delta();
            
            // Apply angular velocity to rotation
            if (!rb.freezeRotation && glm::length(rb.angularVelocity) > 0.0001f)
            {
                float angle = glm::length(rb.angularVelocity) * time.Delta();
                glm::vec3 axis = glm::normalize(rb.angularVelocity);
                transform.RotateAxis(axis, glm::degrees(angle));
            }

            // Reset forces and torques for the next frame
            rb.force = glm::vec3(0.0f);
            rb.torque = glm::vec3(0.0f);
        });
    }
    
    void PhysicsSystem::CollisionSystem(World& world)
    {
        EventManager& eventManager = world.GetResourse<EventManager>();
        
        // Create collections for the different collider types
        auto boxColliders = world.View<BoxCollider, Transform>().GetPacked();
        auto sphereColliders = world.View<SphereCollider, Transform>().GetPacked();
        
        // Combined list of all colliders
        std::vector<std::pair<Entity, Collider*>> allColliders;
        
        // Add box colliders to the combined list
        for (auto& item : boxColliders) {
            Entity entity = item.id;
            BoxCollider& boxCollider = world.Get<BoxCollider>(entity);
            allColliders.push_back({entity, static_cast<Collider*>(&boxCollider)});
        }
        
        // Add sphere colliders to the combined list
        for (auto& item : sphereColliders) {
            Entity entity = item.id;
            SphereCollider& sphereCollider = world.Get<SphereCollider>(entity);
            allColliders.push_back({entity, static_cast<Collider*>(&sphereCollider)});
        }
        
        // Check each collider pair for collisions
        for (size_t i = 0; i < allColliders.size(); i++)
        {
            Entity entityA = allColliders[i].first;
            Collider* colliderA = allColliders[i].second;
            Transform& transformA = world.Get<Transform>(entityA);
            
            for (size_t j = i + 1; j < allColliders.size(); j++)
            {
                Entity entityB = allColliders[j].first;
                Collider* colliderB = allColliders[j].second;
                Transform& transformB = world.Get<Transform>(entityB);
                
                glm::vec3 collisionPoint, collisionNormal;
                float penetrationDepth;
                
                // Check for collision
                if (!colliderA->CheckCollision(colliderB, transformA, transformB,
                    collisionPoint, collisionNormal, penetrationDepth))
                    continue;

                // Create collision data
                CollisionData collisionData;
                collisionData.entityA = entityA;
                collisionData.entityB = entityB;
                collisionData.collisionPoint = collisionPoint;
                collisionData.collisionNormal = collisionNormal;
                collisionData.penetrationDepth = penetrationDepth;

                // If either collider is a trigger, send trigger event
                if (colliderA->isTrigger || colliderB->isTrigger)
                {
                    OnTriggerEnter triggerEvent{ collisionData, world };
                    eventManager.Invoke(triggerEvent, AllowToken<OnTriggerEnter>());
                }
                else
                {
                    // Send collision event
                    OnCollision collisionEvent{ collisionData, world };
                    eventManager.Invoke(collisionEvent, AllowToken<OnCollision>());

                    // Collision resolution for RigidBody components
                    bool hasRbA = world.Has<RigidBody>(entityA);
                    bool hasRbB = world.Has<RigidBody>(entityB);

                    if ((!hasRbA || !hasRbB))
						continue; // Skip if either entity does not have a RigidBody

                    RigidBody& rbA = world.Get<RigidBody>(entityA);
                    RigidBody& rbB = world.Get<RigidBody>(entityB);

                    // Skip if both are kinematic
                    if (rbA.isKinematic && rbB.isKinematic)
                        continue;

                    // Calculate impulse for collision response
                    float invMassSum = rbA.inverseMass + rbB.inverseMass;
                    if (invMassSum <= 0)
						continue; // Avoid division by zero

                    // Position correction (avoid sinking)
                    const float percent = 0.2f; // penetration correction factor
                    glm::vec3 correction = (penetrationDepth / invMassSum) * percent * collisionNormal;

                    if (!rbA.isKinematic)
                        transformA.position -= correction * rbA.inverseMass;

                    if (!rbB.isKinematic)
                        transformB.position += correction * rbB.inverseMass;

                    // Velocity correction (bounce effect)
                    glm::vec3 relativeVelocity = rbB.velocity - rbA.velocity;
                    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
                    // If objects are moving away from each other along the collision normal,
// we need to flip the normal
                    if (velocityAlongNormal > 0)
                    {
                        collisionNormal = -collisionNormal;
                        velocityAlongNormal = -velocityAlongNormal;
                    }
                    // Only resolve if objects are moving toward each other
                    if (velocityAlongNormal < 0)
                    {
                        // Calculate coefficient of restitution (bounciness)
                        float e = std::min(rbA.restitution, rbB.restitution);

                        // Calculate impulse scalar
                        float j = -(1.0f + e) * velocityAlongNormal / invMassSum;

                        // Apply impulse
                        glm::vec3 impulse = j * collisionNormal;

                        if (!rbA.isKinematic)
                            rbA.velocity -= impulse * rbA.inverseMass;

                        if (!rbB.isKinematic)
                            rbB.velocity += impulse * rbB.inverseMass;

                        // Apply friction
                        float friction = (rbA.friction + rbB.friction) * 0.5f;

                        if (friction > 0)
                        {
                            // Recalculate relative velocity after normal impulse
                            relativeVelocity = rbB.velocity - rbA.velocity;

                            // Calculate tangent vector (perpendicular to normal)
                            glm::vec3 tangent = relativeVelocity - (glm::dot(relativeVelocity, collisionNormal) * collisionNormal);
                            float tangentLength = glm::length(tangent);

                            if (tangentLength > 0.0001f)
                            {
                                tangent = tangent / tangentLength;

                                // Calculate friction impulse
                                float jt = -glm::dot(relativeVelocity, tangent) / invMassSum;

                                // Clamp friction
                                jt = glm::clamp(jt, -j * friction, j * friction);

                                // Apply friction impulse
                                glm::vec3 frictionImpulse = jt * tangent;

                                if (!rbA.isKinematic)
                                    rbA.velocity -= frictionImpulse * rbA.inverseMass;

                                if (!rbB.isKinematic)
                                    rbB.velocity += frictionImpulse * rbB.inverseMass;
                            }
                        }
                    }
                }
            }
        }
    }
}