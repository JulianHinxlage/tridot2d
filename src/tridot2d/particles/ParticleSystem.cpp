//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "ParticleSystem.h"
#include "common/Singleton.h"
#include "render/Renderer2D.h"
#include "systems/Time.h"

namespace tridot2d {

	void ParticleSystem::update() {
		time += Singleton::get<Time>()->deltaTime;
		auto &renderer = *Singleton::get<Renderer2D>();
		
		for (int i = 0; i < particles.size(); i++) {
			auto& p = particles[i];

			float t = time - p.createTime;
			if (t > p.lifeTime) {
				particles[i] = particles.back();
				particles.pop_back();
				i--;
				continue;
			}

			glm::vec2 pos = p.position + (p.velocity + p.acceleration * t * 0.5f) * t;
			float rot = p.rotation + p.angular * t;
			glm::vec2 size = p.size + (p.endSize - p.size) * t / p.lifeTime;
			glm::vec4 color = p.startColor + (p.endColor - p.startColor) * t / p.lifeTime;

			float alpha = 1;
			if (t < p.fadeInTime) {
				alpha = t / p.fadeInTime;
			}
			if (t > p.lifeTime - p.fadeOutTime) {
				alpha = 1.0f - (t - (p.lifeTime - p.fadeOutTime)) / p.fadeOutTime;
			}
			color.a *= alpha;

			if (p.type == ParticleType::QUAD) {
				renderer.submitQuad(pos, size, rot, particleDepth, p.texture, Color(color));
			}
			else {
				renderer.submitCircle(pos, size, rot, particleDepth, p.texture, Color(color));
			}
		}
	}

	void ParticleSystem::init() {

	}

	Particle& ParticleSystem::addParticle() {
		if (particles.size() >= maxParticles) {
			if (reuseIndex >= particles.size()) {
				reuseIndex = 0;
			}
			int index = reuseIndex++;
			particles[reuseIndex] = Particle();
			particles[reuseIndex].createTime = time;
			return particles[reuseIndex];
		}
		else {
			particles.push_back({});
			particles.back().createTime = time;
			return particles.back();
		}
	}

}
