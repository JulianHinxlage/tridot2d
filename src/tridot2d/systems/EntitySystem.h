//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "render/Texture.h"
#include "common/Singleton.h"
#include <string>
#include <unordered_map>
#include <memory>

namespace tridot2d {

	class TextureManager {
	public:
		std::string directory;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

		std::shared_ptr<Texture> get(const std::string& filename);
	};

	class Component {
	public:
		virtual void update(class Entity& entity, float deltaTime) {};
		virtual void init(class Entity& entity) {};
	};

	class Entity {
	public:
		std::vector<std::shared_ptr<Component>> components;

		glm::vec2 position = { 0, 0 };
		glm::vec2 scale = { 1, 1 };
		float rotation = 0;

		Entity(const glm::vec2 position = { 0, 0 }, const glm::vec2& scale = { 1, 1 }, float rotation = 0)
			: position(position), scale(scale), rotation(rotation) {}

		template<typename T>
		T *addComponent(const T& t = T()) {
			auto comp = std::make_shared<T>(t);
			components.push_back(comp);
			((Component*)comp.get())->init(*this);
			return comp.get();
		}

		template<typename T>
		T* getComponent() {
			for (auto& comp : components) {
				T* t = dynamic_cast<T*>(comp.get());
				if (t) {
					return t;
				}
			}
			return nullptr;
		}

		void updateComponents(float deltaTime);

		virtual void update(float deltaTime) {};
		virtual void init() {};
	};

	class EntitySystem {
	public:
		std::vector<Entity*> entities;

		void update(float deltaTime);

		template<typename T>
		Entity *addEntity(const T& t = T()) {
			Entity* ent = new T(t);
			entities.push_back(ent);
			ent->init();
			return ent;
		}

		void clear();
	};

	class Sprite : public Component {
	public:
		std::shared_ptr<Texture> texture = nullptr;
		Color color = color::white;

		Sprite(const std::string& texture, Color color = color::white);
		void update(Entity& entity, float deltaTime) override;
	};

	class Velocity : public Component {
	public:
		glm::vec2 velocity = { 0, 0 };
		float angular = 0;

		Velocity(const glm::vec2 velocity = { 0, 0 }, float angular = 0)
			: velocity(velocity), angular(angular) {}

		void update(Entity& ent, float dt) override;
	};

}
