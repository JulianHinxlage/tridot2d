//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Renderer2D.h"
#include <glm/glm.hpp>
#include <string>

namespace tridot2d {

	class TextRenderer {
	public:
		Renderer2D* renderer = nullptr;

		void init();
		void setFont(const std::string& file, int size);
		glm::vec2 getTextSize(const std::string& text, glm::vec2 scale);
		void submit(const std::string& text, glm::vec2 position, glm::vec2 scale = {1, 1}, float rotation = 0, float depth = 0, Color color = color::white);

	private:
		std::shared_ptr<class Atlas> atlas;
	};

}
