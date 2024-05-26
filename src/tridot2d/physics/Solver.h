//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Shape.h"

namespace tridot2d {

	class Solver {
	public:
		class PhysicsSystem* physics = nullptr;
		float deltaTime = 0.01;

		virtual void resolve(Manifold& manifold) {};
		virtual void preUpdate(Body* body) {};
		virtual void postUpdate(Body* body) {};
	};

	class EulerSolver : public Solver {
	public:
		void resolve(Manifold& manifold) override;
		void preUpdate(Body* body) override;
	};

}
