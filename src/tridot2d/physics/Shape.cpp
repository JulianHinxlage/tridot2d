//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Shape.h"

namespace tridot2d {

	bool checkBoxBox(BoxShape* boxA, Body *bodyA, BoxShape* boxB, Body *bodyB, Manifold* manifold) {
		float right = bodyA->position.x + boxA->halfSize.x - (bodyB->position.x - boxB->halfSize.x);
		float left = bodyB->position.x + boxB->halfSize.x - (bodyA->position.x - boxA->halfSize.x);
		float top = bodyA->position.y + boxA->halfSize.y - (bodyB->position.y - boxB->halfSize.y);
		float bottom = bodyB->position.y + boxB->halfSize.y - (bodyA->position.y - boxA->halfSize.y);

		if (right > 0 && left > 0 && top > 0 && bottom > 0) {
			float penetration = glm::min(right, glm::min(left, glm::min(top, bottom)));
			glm::vec2 normal = glm::vec2(0);
			float distA = 0;
			float distB = 0;

			if (penetration == right) {
				normal = { -1, 0 };
				distA = boxA->halfSize.x;
				distB = boxB->halfSize.x;
			}
			else if (penetration == left) {
				normal = { 1, 0 };
				distA = boxA->halfSize.x;
				distB = boxB->halfSize.x;
			}
			else if (penetration == top) {
				normal = { 0, -1 };
				distA = boxA->halfSize.y;
				distB = boxB->halfSize.y;
			}
			else if (penetration == bottom) {
				normal = { 0, 1 };
				distA = boxA->halfSize.y;
				distB = boxB->halfSize.y;
			}


			manifold->a.body = bodyA;
			manifold->a.offset = normal * distA;
			manifold->a.penetration = penetration;
			manifold->a.normal = normal;

			manifold->b.body = bodyB;
			manifold->b.offset = -normal * distB;
			manifold->b.penetration = penetration;
			manifold->b.normal = -normal;

			return true;
		}
		return false;
	}

    bool BoxShape::check(Body* body, Body* otherBody, Shape* otherShape, Manifold* result) {
		if (otherShape->type == ShapeType::BOX) {
			return checkBoxBox(this, body, (BoxShape*)otherShape, otherBody, result);
		}
        return false;
    }

}
