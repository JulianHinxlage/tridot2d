//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "core/EntitySystem.h"
#include "render/Color.h"
#include "components/components.h"

namespace tridot2d {

	class UiElement;

	class UiLayout : public Entity {
	public:
		UiElement* parent;
		std::vector<UiElement*> elements;

		glm::vec2 margin = { 10, 10 };
		glm::vec2 padding = { 10, 10 };
		glm::vec2 offset = { 0, 0 };

		enum Anchor {
			CENTER = 0,
			TOP = 1 << 0,
			LEFT = 1 << 1,
			BOTTOM = 1 << 2,
			RIGHT = 1 << 3,
		};
		int anchor = TOP | LEFT;

		UiLayout(UiElement* parent = nullptr) 
			: parent(parent) {}

		UiLayout(Entity* parent)
			: parent(parent->getComponent<UiElement>()) {}


		void addElement(Entity* entity) {
			addElement(entity->getComponent<UiElement>());
		}

		void addElement(UiElement *element) {
			if (element) {
				elements.push_back(element);
			}
		}

		void update() override;
	};

	class UiElement : public Component {
	public:
		glm::vec2 position;
		glm::vec2 size;

		UiElement(const glm::vec2& size = { 50, 50 }, const glm::vec2& position = { 0, 0 });

		~UiElement();

		void init() override;

		void update() override;
	};

	class UiInteractable : public Component {
	public:
		bool hovered = false;
		bool entered = false;
		bool leaving = false;
		bool pressed = false;
		bool released = false;
		bool down = false;

		void update() override;
	};

	class UiText : public Component {
	public:
		std::string text;
		Color color;
	
		UiText(const std::string& text = "", Color color = color::black)
			: text(text), color(color) {}

		void update() override;
	};

	class UiButton : public Entity {
	public:
		std::function<void()> onClicked;
	
		UiButton(const std::string& text = "", const glm::vec2 size = { 100, 50 }, const glm::vec2 pos = { 0, 0 }, Color color = Color(0xaaaaaa)) {
			addComponent(UiElement(size, pos));
			addComponent(Sprite("", color));
			addComponent(UiText(text));
		}

		void update() override;
	};

	class UiCheckbox : public Entity {
	public:
		bool value = false;
		std::function<void()> onChanged;
	
		void update() override;
	};

	class UiSlider : public Entity {
	public:
		float value = 0.0f;
		float max = 0.0f;
		float min = 0.0f;
		std::function<void()> onChanged;
	
		void update() override;
	};

}
