//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "render/Texture.h"
#include "common/Singleton.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <map>
#include <set>

namespace tridot2d {

	class Component {
	public:
		virtual ~Component() {}
		virtual void update(class Entity& entity, float deltaTime) {};
		virtual void init(class Entity& entity) {};
	};

	class Entity {
	public:
		std::vector<std::shared_ptr<Component>> components;
		bool active = true;

		glm::vec2 position = { 0, 0 };
		glm::vec2 scale = { 1, 1 };
		float rotation = 0;

		Entity(const glm::vec2 position = { 0, 0 }, const glm::vec2& scale = { 1, 1 }, float rotation = 0)
			: position(position), scale(scale), rotation(rotation) {}

		virtual ~Entity() {}

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

	private:
		int entityIndex = -1;
		friend class EntitySystem;
	};

	class EntityRef {
	public:
		EntityRef();

		EntityRef(Entity* ent);

		EntityRef(const EntityRef& ref);

		EntityRef(EntityRef&& ref);

		~EntityRef();

		void operator=(const EntityRef& ref);

		void operator=(Entity* ent);

		bool operator==(Entity* ent);

		bool operator==(const EntityRef& ref);

		operator bool();

		operator Entity*();

		Entity* operator->();

		Entity& operator*();

		Entity* get();

		void set(Entity *ent);

		static void invalidate(Entity* ent);

	private:
		Entity* entity = nullptr;
		static std::map<Entity*, std::set<EntityRef*>> refs;
	};

	class EntitySystem {
	public:
		std::vector<Entity*> entities;
		std::vector<Entity*> pendingRemoves;
		std::vector<Entity*> pendingAdds;

		void update(float deltaTime);

		template<typename T>
		Entity* addEntity(T* ent) {
			pendingAdds.push_back(ent);
			return ent;
		}

		template<typename T>
		Entity *addEntity(const T& t = T()) {
			Entity* ent = new T(t);
			pendingAdds.push_back(ent);
			return ent;
		}

		void removeEntity(Entity* ent);
		void clear();
	};

}
