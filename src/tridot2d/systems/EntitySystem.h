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
		void addComponent(const T& t = T()) {
			components.push_back(std::make_shared<T>(t));
		}

		template<typename T>
		T* getComponent() {
			for (auto& comp : components) {
				T* t = dynamic_cast<T*>(comp);
				if (t) {
					return t;
				}
			}
			return nullptr;
		}

		void updateComponents(float deltaTime);
		void initComponents();

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
			ent->initComponents();
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
