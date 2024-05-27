//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "commonComponents.h"
#include "render/Renderer2D.h"
#include "systems/Time.h"

namespace tridot2d {

	std::shared_ptr<Texture> TextureManager::get(const std::string& filename) {
		auto i = textures.find(filename);
		if (i != textures.end()) {
			return i->second;
		}
		if (filename == "") {
			textures[filename] = nullptr;
			return nullptr;
		}
		auto texture = std::make_shared<Texture>();
		texture->load(directory + filename);
		textures[filename] = texture;
		return texture;
	};

	Sprite::Sprite(const std::string& texture, Color color) {
		this->color = color;
		this->texture = Singleton::get<TextureManager>()->get(texture);
	}

	void Sprite::update(Entity& entity, float deltaTime) {
		Singleton::get<Renderer2D>()->submitQuad(entity.position, entity.scale, entity.rotation, texture.get(), color, coords1, coords2);
	};

	void Velocity::update(Entity& ent, float dt) {
		ent.position += velocity * dt;
		ent.rotation += angular * dt;
	}

	LifeTime::LifeTime(float time) {
		timeLeft = time;
	}

	void LifeTime::update(Entity& ent, float deltaTime) {
		timeLeft -= deltaTime;
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

}
