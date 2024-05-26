//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "PhysicsSystem.h"

namespace tridot2d {
	
	void PhysicsSystem::init() {
		broadPhase = std::make_shared<StaticGridBroadPhase>(glm::vec2(2, 2), 50, 50);
		broadPhase->physics = this;
		solver = std::make_shared<EulerSolver>();
		solver->physics = this;
		defaultShape = std::make_shared<BoxShape>();
	}

	void PhysicsSystem::update(float deltaTime, int subSteps) {
		for (int i = 0; i < subSteps; i++) {
			step(deltaTime / subSteps);
		}
	}

	void PhysicsSystem::step(float deltaTime) {
		solver->deltaTime = deltaTime;

		for (auto& body : bodies) {
			broadPhase->updateBody(body.get());
			solver->preUpdate(body.get());
		}

		broadPhase->each([&](Body* a, Body* b) {
			Manifold manifold;
			if (a->shape->check(a, b, b->shape, &manifold)) {
				if (manifold.a.body->type != BodyType::COLLIDER && manifold.b.body->type != BodyType::COLLIDER) {
					solver->resolve(manifold);
				}
				if (manifold.a.body->onCollide) {
					manifold.a.body->onCollide(manifold.b.body, manifold.a);
				}
				if (manifold.b.body->onCollide) {
					manifold.b.body->onCollide(manifold.a.body, manifold.b);
				}
			}
		});

		for (auto& body : bodies) {
			solver->postUpdate(body.get());
		}
	}

	Body* PhysicsSystem::addBody() {
		auto body = std::make_shared<Body>();
		body->index = bodies.size();
		body->shape = defaultShape.get();
		bodies.push_back(body);
		return body.get();
	}

	void PhysicsSystem::removeBody(Body* body) {
		broadPhase->removeBody(body);
		int index = body->index;
		body->index = 0;
		if (index >= 0 && index < bodies.size()) {
			bodies[index] = nullptr;
		}
	}

	void PhysicsSystem::clearBodies() {
		broadPhase->clearBodies();
		bodies.clear();
	}

	int PhysicsSystem::getBodyCount() {
		return bodies.size();
	}

	Body* PhysicsSystem::getBody(int index) {
		if (index >= 0 && index < bodies.size()) {
			return bodies[index].get();
		}
		return nullptr;
	}

}
