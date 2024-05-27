//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "systems/EntitySystem.h"

namespace tridot2d {

	class TextureManager {
	public:
		std::string directory;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

		std::shared_ptr<Texture> get(const std::string& filename);
	};

	class Sprite : public Component {
	public:
		std::shared_ptr<Texture> texture = nullptr;
		Color color = color::white;
		glm::vec2 coords1 = { 0, 0 };
		glm::vec2 coords2 = { 1, 1 };

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

	class LifeTime : public Component {
	public:
		float timeLeft = 0;

		LifeTime(float time = 1);

		void update(Entity& ent, float deltaTime) override;
	};

	class SpriteSheet : public Component {
	public:
		std::shared_ptr<Texture> texture = nullptr;
		glm::vec2 resoultion = { 0, 0 };
		glm::vec2 spritePadding = { 0, 0 };
		glm::vec2 spriteSize = { 0, 0 };
		glm::vec2 spriteOffset = { 0, 0 };

		SpriteSheet(const std::string& texture);
		void set(Sprite *sprite, int index);
	};

}
