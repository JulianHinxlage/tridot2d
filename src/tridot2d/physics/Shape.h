//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Body.h"
#include <glm/glm.hpp>

namespace tridot2d {

	enum class ShapeType {
		POINT,
		BOX,
		CIRCLE,
		POLYGON,
	};

	class Shape {
	public:
		ShapeType type;
		glm::vec2 offset = { 0, 0 };
		virtual bool check(Body *body, Body *otherBody, Shape* otherShape, Manifold* result) { return false; };
	};

	class BoxShape : public Shape {
	public:
		glm::vec2 halfSize = { 0.5, 0.5 };
		BoxShape(const glm::vec2 halfSize = { 0.5, 0.5 })
			: halfSize(halfSize) {
			type = ShapeType::BOX;
		}

		bool check(Body* body, Body* otherBody, Shape* otherShape, Manifold* result) override;
	};

}
