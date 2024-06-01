//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "components.h"
#include "systems/AssetsManager.h"
#include "render/Renderer2D.h"
#include "common/Singleton.h"
#include "systems/Time.h"

namespace tridot2d {

	Sprite::Sprite(const std::string& texture, Color color) {
		this->color = color;
		this->texture = Singleton::get<TextureManager>()->get(texture);
	}

	void Sprite::update(Entity& entity) {
		Singleton::get<Renderer2D>()->submitQuad(entity.position + offset, entity.scale * scale, entity.rotation + rotation, depth, texture.get(), color, coords1, coords2);
	};

	void Velocity::update(Entity& ent) {
		float dt = Singleton::get<Time>()->deltaTime;
		ent.position += velocity * dt;
		ent.rotation += angular * dt;
	}

	LifeTime::LifeTime(float time) {
		timeLeft = time;
	}

	void LifeTime::update(Entity& ent) {
		float dt = Singleton::get<Time>()->deltaTime;
		timeLeft -= dt;
		if (timeLeft <= 0) {
			Singleton::get<EntitySystem>()->removeEntity(&ent);
		}
	}

	SpriteSheet::SpriteSheet(const std::string& texture) {
		this->texture = Singleton::get<TextureManager>()->get(texture);
	}

	void SpriteSheet::set(Sprite* sprite, int index) {
		int xCount = resoultion.x / spritePadding.x;
		int yCount = resoultion.y / spritePadding.y;

		int x = index % xCount;
		int y = (index / yCount) % yCount;

		sprite->texture = texture;
		sprite->coords1.x = (spritePadding.x * x + spriteOffset.x) / resoultion.x;
		sprite->coords1.y = (spritePadding.y * y + spriteOffset.y) / resoultion.y;
		sprite->coords2 = sprite->coords1 + (spriteSize / resoultion);
	}

	SpriteAnimation::SpriteAnimation(Sprite* sprite) {
		this->sprite = sprite;
		spriteSheet.texture = sprite->texture;
	}

	void SpriteAnimation::set(int animationId) {
		if (animationId != currentAnimationId) {
			currentAnimationId = animationId;
			time = 0;
		}
	}

	SpriteAnimation::Animation &SpriteAnimation::add(int animationId, std::vector<int> spriteIndices, float time, bool loop) {
		Animation anim;
		anim.animationId = animationId;
		anim.spriteIndices = spriteIndices;
		anim.time = time;
		anim.loop = loop;
		animations[animationId] = anim;
		return animations[animationId];
	}

	void SpriteAnimation::update(Entity& ent) {
		auto i = animations.find(currentAnimationId);
		if (i != animations.end()) {
			Animation &anim = i->second;
			if (anim.loop) {
				if (time > anim.time) {
					time -= anim.time;
				}
			}
			int index = (time / anim.time) * anim.spriteIndices.size();
			if (index >= 0 && index < anim.spriteIndices.size()) {
				int spriteIndex = anim.spriteIndices[index];
				if (spriteIndex >= 0) {
					spriteSheet.set(sprite, spriteIndex);
				}
				else {
					spriteSheet.set(sprite, -spriteIndex);
					std::swap(sprite->coords1.x, sprite->coords2.x);
				}
			}
		}
		time += Singleton::get<Time>()->deltaTime;
	}

}
