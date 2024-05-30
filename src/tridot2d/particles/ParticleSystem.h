//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "render/Texture.h"
#include "Particle.h"
#include <glm/glm.hpp>

namespace tridot2d {

	class ParticleSystem {
	public:
		float particleDepth = 10;

		void update(float deltaTime);
		void init();
		Particle &addParticle();
	private:
		std::vector<Particle> particles;
		float time = 0;
		int maxParticles = 10000;
		int reuseIndex = 0;
	};

}
