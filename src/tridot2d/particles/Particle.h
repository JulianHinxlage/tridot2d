//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "render/Texture.h"
#include <glm/glm.hpp>

namespace tridot2d {

	enum ParticleType {
		QUAD,
		CIRCLE,
	};

	class Particle {
	public:
		glm::vec2 position = { 0, 0 };
		glm::vec2 size = { 0.1, 0.1 };
		float rotation = 0;

		glm::vec2 velocity = { 0, 0 };
		float angular = 0;
		glm::vec2 acceleration = { 0, 0 };
		glm::vec2 endSize = { 0.1, 0.1 };
		float createTime = 0;

		glm::vec4 startColor = { 1, 1, 1, 1 };
		glm::vec4 endColor = { 1, 1, 1, 1 };

		float lifeTime = 0;

		float fadeInTime = 0;
		float fadeOutTime = 0;

		ParticleType type = ParticleType::QUAD;
		Texture* texture = nullptr;
	};

}