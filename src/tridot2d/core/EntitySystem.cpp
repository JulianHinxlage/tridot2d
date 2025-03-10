//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "EntitySystem.h"
#include "render/Renderer2D.h"
#include "common/Singleton.h"

namespace tridot2d {

	std::map<Entity*, std::set<EntityRef*>> EntityRef::refs;

	void Entity::removeEntity() {
		entitySystem->removeEntity(this);
	}

	Entity::Entity(const Entity& ent) {
		components = ent.components;
		active = ent.active;
		entitySystem = ent.entitySystem;
		position = ent.position;
		scale = ent.scale;
		rotation = ent.rotation;
		entityIndex = ent.entityIndex;
		for (auto& comp : components) {
			comp->entity = this;
		}
	}

	EntityRef::EntityRef() {
		set(nullptr);
	}

	EntityRef::EntityRef(Entity* ent) {
		set(ent);
	}

	EntityRef::EntityRef(const EntityRef& ref) {
		set(ref.entity);
	}

	EntityRef::EntityRef(EntityRef&& ref) {
		set(ref.entity);
	}

	EntityRef::~EntityRef() {
		set(nullptr);
	}

	void EntityRef::operator=(const EntityRef& ref) {
		set(ref.entity);
	}

	void EntityRef::operator=(Entity* ent) {
		set(ent);
	}

	bool EntityRef::operator==(Entity* ent) {
		return ent == entity;
	}

	bool EntityRef::operator==(const EntityRef& ref) {
		return ref.entity == entity;
	}

	EntityRef::operator bool() {
		return (bool)entity;
	}

	EntityRef::operator Entity* () {
		return entity;
	}

	Entity* EntityRef::operator->() {
		return entity;
	}

	Entity& EntityRef::operator*() {
		return *entity;
	}

	Entity* EntityRef::get() {
		return entity;
	}

	void EntityRef::set(Entity* ent) {
		if (entity != nullptr) {
			refs[entity].erase(this);
			entity = nullptr;
		}
		entity = ent;
		if (entity != nullptr) {
			refs[entity].insert(this);
		}
	}

	void EntityRef::invalidate(Entity* ent) {
		if (refs.find(ent) != refs.end()) {
			for (auto& i : refs[ent]) {
				if (i->entity == ent) {
					i->entity = nullptr;
				}
			}
			refs.erase(ent);
		}
	}

	EntitySystem::~EntitySystem() {
		clear();
	}

	void EntitySystem::updatePending() {
		for (auto* ent : pendingRemoves) {
			int index = ent->entityIndex;
			if (index >= 0 && index < entities.size()) {
				EntityRef::invalidate(ent);
				Entity* e = entities[index];
				entities[index] = entities.back();
				entities[index]->entityIndex = index;
				entities.pop_back();
				delete e;
			}
		}
		pendingRemoves.clear();

		for (auto* ent : pendingAdds) {
			ent->entityIndex = entities.size();
			for (int i = 0; i < ent->components.size(); i++) {
				auto* comp = ent->components[i].get();
				if (comp) {
					comp->init();
				}
			}
			ent->entitySystem = this;
			ent->init();
			entities.push_back(ent);
		}
		pendingAdds.clear();
	}

	void EntitySystem::update() {
		updatePending();

		for (auto& entity : entities) {
			if (entity) {
				if (entity->active) {
					entity->preUpdate();
					for (auto& comp : entity->components) {
						if (comp) {
							comp->update();
						}
					}
					entity->update();
				}
			}
		}
	}

	void EntitySystem::removeEntity(Entity* ent) {
		pendingRemoves.push_back(ent);
	}

	void EntitySystem::clear() {
		for (auto& entity : entities) {
			if (entity) {
				EntityRef::invalidate(entity);
				delete entity;
				entity = nullptr;
			}
		}
		entities.clear();
		for (auto& entity : pendingAdds) {
			if (entity) {
				EntityRef::invalidate(entity);
				delete entity;
				entity = nullptr;
			}
		}
		pendingAdds.clear();
		pendingRemoves.clear();
	}



}