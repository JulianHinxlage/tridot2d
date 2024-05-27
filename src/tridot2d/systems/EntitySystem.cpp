//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "EntitySystem.h"
#include "render/Renderer2D.h"
#include "common/Singleton.h"

namespace tridot2d {

	void Entity::updateComponents(float deltaTime) {
		for (auto& comp : components) {
			if (comp) {
				comp->update(*this, deltaTime);
			}
		}
	}

	void EntitySystem::update(float deltaTime) {
		for (auto* ent : pendingRemoves) {
			if (ent->entityIndex >= 0 && ent->entityIndex < entities.size()) {
				Entity* tmp = entities[ent->entityIndex];
				entities[ent->entityIndex] = entities.back();
				entities[ent->entityIndex]->entityIndex = ent->entityIndex;
				entities.pop_back();
				delete tmp;
			}
		}
		pendingRemoves.clear();

		for (auto* ent : pendingAdds) {
			ent->entityIndex = entities.size();
			ent->init();
			entities.push_back(ent);
		}
		pendingAdds.clear();

		for (auto& entity : entities) {
			entity->updateComponents(deltaTime);
			entity->update(deltaTime);
		}
	}

	void EntitySystem::removeEntity(Entity* ent) {
		pendingRemoves.push_back(ent);
	}

	void EntitySystem::clear() {
		for (auto& entity : entities) {
			delete entity;
			entity = nullptr;
		}
		entities.clear();
	}

}