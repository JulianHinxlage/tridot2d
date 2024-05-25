//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

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
		class Entity* followEntity = nullptr;
		float followSpeed = -1;
		glm::vec2 followDeadZone = { 0, 0 };

		virtual glm::mat4 getMatrix();
		virtual void update(float deltaTime);

	private:
		glm::vec2 mousePos;
	};

}
