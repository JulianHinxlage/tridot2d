//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>

namespace tridot2d {

	enum class BodyType {
		STATIC,
		DYNAMIC,
		COLLIDER,
	};

	class Manifold {
	public:
		class Point {
		public:
			class Body* body = nullptr;
			glm::vec2 offset = { 0, 0 };
			glm::vec2 normal = { 1, 0 };
			float penetration = 0;
		};

		Point a;
		Point b;
	};

	class Body {
	public:
		BodyType type = BodyType::STATIC;

		glm::vec2 position = { 0, 0 };
		glm::vec2 scale = { 1, 1 };
		float rotation = 0;

		glm::vec2 velocity = { 0, 0 };
		float angular = 0;

		glm::vec2 force = { 0, 0 };
		glm::vec2 drag = { 0, 0 };
		float bounciness = 0;
		glm::vec2 gravity = { 0, 0 };

		float mass = 1;
		int index = 0;
		class Entity *entity = nullptr;
		class Shape* shape = nullptr;

		std::function<void(Body *, Manifold::Point)> onCollide = nullptr;
	};

}
