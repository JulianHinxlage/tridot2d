//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "systems/EntitySystem.h"
#include <map>

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
		float depth = 0;
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

	class SpriteSheet {
	public:
		std::shared_ptr<Texture> texture = nullptr;
		glm::vec2 resoultion = { 1, 1 };
		glm::vec2 spritePadding = { 1, 1 };
		glm::vec2 spriteSize = { 1, 1 };
		glm::vec2 spriteOffset = { 0, 0 };

		SpriteSheet() {};
		SpriteSheet(const std::string& texture);
		void set(Sprite *sprite, int index);

		void setup(int xCount, int yCount, glm::vec2 margin = {0, 0}) {
			spritePadding = resoultion / glm::vec2(xCount, yCount);
			spriteSize = spritePadding;
			spriteOffset = spriteSize * margin;
			spriteSize -= spriteOffset * 2.0f;
		}
	};

	class SpriteAnimation : public Component {
	public:
		Sprite* sprite = nullptr;
		SpriteSheet spriteSheet;

		class Animation {
		public:
			int animationId = -1;
			float time = 1;
			bool loop = true;
			std::vector<int> spriteIndices;
		};
		std::map<int, Animation> animations;
		int currentAnimationId = -1;
		float time = 0;

		SpriteAnimation(Sprite* sprite);

		Animation& add(int animationId, std::vector<int> spriteIndices = {}, float time = 1, bool loop = false);
		void set(int animationId);

		void update(Entity& ent, float detaTime) override;
	};

}
