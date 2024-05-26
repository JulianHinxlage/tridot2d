//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Body.h"
#include "systems/EntitySystem.h"

namespace tridot2d {

	class RigidBody : public Component {
	public:
		Body* body = nullptr;
		float mass = 1;

		RigidBody(float mass = 1) {
			this->mass = mass;
		}

		~RigidBody() {
			if (body) {
				Singleton::get<PhysicsSystem>()->removeBody(body);
			}
		}

		void init(Entity& ent) override {
			body = Singleton::get<PhysicsSystem>()->addBody();
			body->mass = mass;
			body->drag = { 1, 1 };
			body->bounciness = 0;
			if (mass == 0) {
				body->type = BodyType::STATIC;
			}
			else {
				body->type = BodyType::DYNAMIC;
			}
			body->position = ent.position;
			body->rotation = ent.rotation;
			body->userPtr = &ent;
		}

		void update(Entity& ent, float dt) override {
			ent.position = body->position;
			ent.rotation = body->rotation;
		}
	};

}