//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Shape.h"

namespace tridot2d {

	bool checkBoxBox(BoxShape* boxA, Body *bodyA, BoxShape* boxB, Body *bodyB, Manifold* manifold) {
		glm::vec2 posA = bodyA->position + boxA->offset;
		glm::vec2 posB = bodyB->position + boxB->offset;
		glm::vec2 sizeA = boxA->halfSize * bodyA->scale;
		glm::vec2 sizeB = boxB->halfSize * bodyB->scale;

		float right = posA.x + sizeA.x - (posB.x - sizeB.x);
		float left = posB.x + sizeB.x - (posA.x - sizeA.x);
		float top = posA.y + sizeA.y - (posB.y - sizeB.y);
		float bottom = posB.y + sizeB.y - (posA.y - sizeA.y);

		if (right > 0 && left > 0 && top > 0 && bottom > 0) {
			float penetration = glm::min(right, glm::min(left, glm::min(top, bottom)));
			glm::vec2 normal = glm::vec2(0);
			float distA = 0;
			float distB = 0;

			if (penetration == right) {
				normal = { -1, 0 };
				distA = sizeA.x;
				distB = sizeB.x;
			}
			else if (penetration == left) {
				normal = { 1, 0 };
				distA = sizeA.x;
				distB = sizeB.x;
			}
			else if (penetration == top) {
				normal = { 0, -1 };
				distA = sizeA.y;
				distB = sizeB.y;
			}
			else if (penetration == bottom) {
				normal = { 0, 1 };
				distA = sizeA.y;
				distB = sizeB.y;
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
