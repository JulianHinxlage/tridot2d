//
// Copyright (c) 2023 Julian Hinxlage. All rights reserved.
//

#include "UiElement.h"
#include "render/Renderer2D.h"
#include "render/TextRenderer.h"
#include "systems/Input.h"
#include "systems/Camera.h"
#include "common/Singleton.h"
#include <queue>

namespace tridot2d {

	void UiLayout::update() {
		auto* camera = Singleton::get<Camera>();

		glm::vec2 center = camera->resolution / 2.0f;

		glm::vec2 cursor = center + offset;
		glm::vec2 advand = { 0.0f, padding.y };
		glm::vec2 advandFactor = { 0.0f, 1.0f };
		glm::vec2 sizeFactor = { -0.5, 0.0f };
		
		if (anchor & TOP) {
			cursor.y -= center.y;
			cursor.y += margin.y;
			sizeFactor.y = 0;
		}
		if (anchor & BOTTOM) {
			cursor.y += center.y;
			cursor.y -= margin.y;

			advand.y = -padding.y;
			advandFactor.y = -1.0f;
			sizeFactor.y = -1;
		}
		if (anchor & LEFT) {
			cursor.x -= center.x;
			cursor.x += margin.x;
			sizeFactor.x = 0;
		}
		if (anchor & RIGHT) {
			cursor.x += center.x;
			cursor.x -= margin.x;
			sizeFactor.x = -1;
		}

		glm::vec2 start = cursor;
		for (auto* e : elements) {
			e->position = cursor + e->size * sizeFactor;
			cursor += advand + e->size.y * advandFactor;
		}
		glm::vec2 end = cursor;

		if (!(anchor & (TOP | BOTTOM))) {
			offset.y -= (end.y + start.y) / 2.0f - center.y;
		}
		if (!(anchor & (LEFT | RIGHT))) {
			offset.x -= (end.x + start.x) / 2.0f - center.x;
		}
	}

	UiElement::UiElement(const glm::vec2& size, const glm::vec2& position)
		: position(position), size(size) {}

	UiElement::~UiElement() {}

	void UiElement::init() {}

	void UiElement::update() {
		Camera* camera = Singleton::get<Camera>();
		entity->position.x = position.x;
		entity->position.y = camera->resolution.y - position.y;
		entity->position.x += size.x * 0.5f;
		entity->position.y -= size.y * 0.5f;
		entity->scale = size;
	}

	void UiText::update() {
		glm::vec2 scale = glm::vec2(entity->scale.y, entity->scale.y) * 0.6f;
		glm::vec2 size = Singleton::get<TextRenderer>()->getTextSize(text, scale);
		float factor = 0.9f;
		if (size.x > entity->scale.x) {
			factor = entity->scale.x / size.x * 0.9f;
		}
		Singleton::get<TextRenderer>()->submit(text, entity->position - glm::vec2(size.x * factor * 0.5f, size.y * factor * 0.5f), scale * factor, entity->rotation, -12, color);
	}

	void UiButton::update() {

	}

}
