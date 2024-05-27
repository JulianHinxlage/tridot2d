//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Camera.h"
#include "EntitySystem.h"
#include "common/Singleton.h"
#include "Input.h"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace tridot2d {

	glm::mat4 Camera::getMatrix() {
		glm::mat4 matrix(1);
		matrix = glm::scale(matrix, glm::vec3((1.0f / scale.x) / aspectRatio, (1.0f / scale.y), 1));
		matrix = glm::translate(matrix, glm::vec3(-position, 0));
		return matrix;
	}

	void Camera::update(float deltaTime) {
		Input& input = *Singleton::get<Input>();

		if (mouseMoveEnabled) {
			if (input.pressed(Input::Button::MOUSE_BUTTON_RIGHT)) {
				mousePos.x = input.getMousePosition().x;
				mousePos.y = -input.getMousePosition().y;
			}
			else if (input.down(Input::Button::MOUSE_BUTTON_RIGHT)) {
				glm::vec2 newPos;
				newPos.x = input.getMousePosition().x;
				newPos.y = -input.getMousePosition().y;
				position -= (newPos - mousePos) * 0.002f * scale * mouseSpeed;
				mousePos = newPos;
			}
		}
		if (mouseZoomEnabled) {
			if (input.getMouseWheelDelta() != 0) {
				scale *= 1.0f - input.getMouseWheelDelta() * 0.1;
			}
		}
		if (followEnabled) {
			if (followEntity) {
				float factor = deltaTime * followSpeed;
				if (followSpeed == -1) {
					factor = 1;
				}
				if (factor < 0) {
					factor = 0;
				}
				if (factor > 1) {
					factor = 1;
				}
				
				position += (followEntity->position - position) * factor;
			}
		}
	}

}
