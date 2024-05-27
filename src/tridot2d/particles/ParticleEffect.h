//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "render/Texture.h"
#include "Particle.h"
#include <glm/glm.hpp>

namespace tridot2d {

	class ParticleEffect {
	public:
		int particlesPerTrigger = 10;
		float particlesPerSecond = 0;

		glm::vec2 postionVariance = { 0, 0 };
		float direction = 0;
		float directionVariance = glm::radians(360.0f);
		float speed = 1;
		float speedVariance = 0.5;
		glm::vec2 acceleration = {0, 0};


		float rotation = 0;
		float rotationVariance = 0;
		float angular = 0;
		float angularVariance = 0;

		glm::vec2 startSize = { 0.1, 0.1 };
		glm::vec2 endSize = { 0.1, 0.1 };
		glm::vec2 startSizeVariance = { 0.0, 0.0 };
		glm::vec2 endSizeVariance = { 0.0, 0.0 };

		glm::vec4 startColor = { 1, 1, 1, 1 };
		glm::vec4 endColor = { 1, 1, 1, 1 };
		glm::vec4 startColorVariance = { 0, 0, 0, 0 };
		glm::vec4 endColorVariance = { 0, 0, 0, 0};

		float lifeTime = 1;
		float lifeTimeVariance = 0;

		float fadeOutTime = 0.2;
		float fadeOutTimeVariance = 0;

		float fadeInTime = 0.2;
		float fadeInTimeVariance = 0;

		Texture* texture = nullptr;
		ParticleType type = ParticleType::QUAD;

		void trigger(const glm::vec2 &position);
		void update(const glm::vec2& position);

	private:
		void createParticle(const glm::vec2& position);
	};

}
