//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Solver.h"

namespace tridot2d {

    void EulerSolver::resolve(Manifold& manifold) {
        float factorA = 0.5f;
        float factorB = 0.5f;

        if (manifold.a.body->type == BodyType::STATIC) {
            factorA = 0;
            factorB = 1;
            if (manifold.b.body->type == BodyType::STATIC) {
                factorA = 0;
                factorB = 0;
            }
        }
        else if (manifold.b.body->type == BodyType::STATIC) {
            factorA = 1;
            factorB = 0;
        }
        else {
            float totalMass = manifold.a.body->mass + manifold.b.body->mass;
            factorA = manifold.b.body->mass / totalMass;
            factorB = manifold.a.body->mass / totalMass;
        }

        manifold.a.body->position += manifold.a.normal * manifold.a.penetration * factorA;
        manifold.b.body->position += manifold.b.normal * manifold.b.penetration * factorB;

        glm::vec2 relativeVelocity = manifold.a.body->velocity - manifold.b.body->velocity;
        float normalVelocity = -glm::dot(manifold.a.normal, relativeVelocity);
        if (normalVelocity > 0) {
            manifold.a.body->velocity += manifold.a.normal * normalVelocity * (1.0f + manifold.a.body->bounciness) * factorA;
            manifold.b.body->velocity += manifold.b.normal * normalVelocity * (1.0f + manifold.b.body->bounciness) * factorB;
        }
    }

    void EulerSolver::preUpdate(Body* body) {
        body->force -= body->velocity * body->drag;
        body->force += body->gravity;
        if (body->type == BodyType::DYNAMIC || body->type == BodyType::COLLIDER) {
            if (body->type == BodyType::DYNAMIC) {
                body->velocity += body->force * deltaTime;
            }
            body->position += body->velocity * deltaTime;
            body->rotation = body->angular * deltaTime;
        }
        else {
            body->velocity = {0, 0};
        }
        body->force = { 0, 0 };
    }

}
