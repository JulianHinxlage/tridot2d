//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "EntitySystem.h"
#include "render/Renderer2D.h"
#include "common/Singleton.h"

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

	void Entity::updateComponents(float deltaTime) {
		for (auto& comp : components) {
			if (comp) {
				comp->update(*this, deltaTime);
			}
		}
	}

	void Entity::initComponents() {
		for (auto& comp : components) {
			if (comp) {
				comp->init(*this);
			}
		}
	}

	void EntitySystem::update(float deltaTime) {
		for (auto& entity : entities) {
			entity->updateComponents(deltaTime);
			entity->update(deltaTime);
		}
	}

	void EntitySystem::clear() {
		for (auto& entity : entities) {
			delete entity;
			entity = nullptr;
		}
		entities.clear();
	}

	Sprite::Sprite(const std::string& texture, Color color) {
		this->color = color;
		this->texture = Singleton::get<TextureManager>()->get(texture);
	}

	void Sprite::update(Entity& entity, float deltaTime) {
		Singleton::get<Renderer2D>()->submitQuad(entity.position, entity.scale, entity.rotation, texture.get(), color);
	};

	void Velocity::update(Entity& ent, float dt) {
		ent.position += velocity * dt;
		ent.rotation += angular * dt;
	}

}