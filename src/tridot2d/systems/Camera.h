//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "EntitySystem.h"
#include <glm/glm.hpp>

namespace tridot2d {

	class Camera {
	public:
		glm::vec2 position = { 0, 0 };
		glm::vec2 scale = { 1, 1 };
		float aspectRatio = 1;

		//mouse
		float mouseSpeed = 1;
		bool mouseMoveEnabled = false;
		bool mouseZoomEnabled = false;

		//follow
		bool followEnabled = false;
		EntityRef followEntity = nullptr;
		float followSpeed = -1;
		glm::vec2 followDeadZone = { 0, 0 };

		glm::mat4 getMatrix();
		void update(float deltaTime);
		glm::vec2 getMousePosition();

	private:
		glm::vec2 mousePos;
	};

}
