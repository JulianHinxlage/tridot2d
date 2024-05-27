//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "ParticleEffect.h"
#include "ParticleSystem.h"
#include "systems/Time.h"
#include "common/Singleton.h"
#include "systems/Random.h"
#include <glm/gtc/matrix_transform.hpp>

namespace tridot2d {

	void ParticleEffect::trigger(const glm::vec2& position) {
		for (int i = 0; i < particlesPerTrigger; i++) {
			createParticle(position);
		}
	}

	void ParticleEffect::update(const glm::vec2& position) {
		int count = Singleton::get<Time>()->deltaTicks(1.0f / particlesPerSecond);
		for (int i = 0; i < count; i++) {
			createParticle(position);
		}
	}

	void ParticleEffect::createParticle(const glm::vec2& position) {
		auto& particleSystem = *Singleton::get<ParticleSystem>();
		auto& random = *Singleton::get<Random>();
		Particle &p = particleSystem.addParticle();

		p.position = position + postionVariance * (random.getVec2() * 2.0f - 1.0f);

		float dir = direction + directionVariance * random.getFloat(-1, 1);
		float vel = speed + speedVariance * random.getFloat(-1, 1);
		p.velocity = (glm::rotate(glm::mat4(1), -dir, glm::vec3(0, 0, 1)) * glm::vec4(vel, 0, 0, 0));
		
		p.rotation = rotation + rotationVariance * random.getFloat(-1, 1);
		p.angular = angular + angularVariance * random.getFloat(-1, 1);
		p.lifeTime = lifeTime + lifeTimeVariance * random.getFloat();
		p.fadeOutTime = fadeOutTime + fadeOutTimeVariance * random.getFloat();
		p.fadeInTime = fadeInTime + fadeInTimeVariance * random.getFloat();
		p.acceleration = acceleration;
		p.size = startSize + startSizeVariance * (random.getVec2() * 2.0f - 1.0f);
		p.endSize = endSize + endSizeVariance * (random.getVec2() * 2.0f - 1.0f);

		p.startColor = startColor + startColorVariance * (random.getVec4() * 2.0f - 1.0f);
		p.endColor = endColor + endColorVariance * (random.getVec4() * 2.0f - 1.0f);
		p.texture = texture;
		p.type = type;
	}

}
