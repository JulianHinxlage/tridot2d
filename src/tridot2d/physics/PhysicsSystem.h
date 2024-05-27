//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Shape.h"
#include "BroadPhase.h"
#include "Solver.h"
#include <vector>
#include <memory>

namespace tridot2d {

	class PhysicsSystem {
	public:
		std::shared_ptr<Shape> defaultShape;

		void init();
		void update(float deltaTime, int subSteps);
		void step(float deltaTime);

		Body *addBody();
		void removeBody(Body* body);
		void clearBodies();

		int getBodyCount();
		Body* getBody(int index);

	private:
		std::vector<std::shared_ptr<Body>> bodies;
		std::shared_ptr<BroadPhase> broadPhase = nullptr;
		std::shared_ptr<Solver> solver = nullptr;
	};
}
