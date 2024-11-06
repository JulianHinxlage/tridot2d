//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Body.h"
#include "PhysicsSystem.h"
#include "core/EntitySystem.h"
#include "common/Singleton.h"

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

		void init() override {
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
			body->position = entity->position;
			body->rotation = entity->rotation;
			body->scale = entity->scale;
			body->entity = entity;
		}

		void update() override {
			entity->position = body->position;
			entity->rotation = body->rotation;
			body->scale = entity->scale;
		}
	};

}
