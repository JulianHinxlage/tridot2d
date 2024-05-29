//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "SimpleRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

static const char* shaderSource =
{
#include "shader/mesh.glsl"
};


namespace tridot2d {

	void SimpleRenderer::init(bool useFrameBuffer, int resolutionX, int resolutionY) {
		shader = std::make_shared<Shader>();
		shader->loadFromSource(shaderSource);

		defaultTexture = std::make_shared<Texture>();
		defaultTexture->create(1, 1);
		Color color = color::white;
		defaultTexture->setData(&color, 1);

		defaultMesh = std::make_shared<Mesh>();
		float vertecies[] = {
			-0.5f, -0.5f, 0.0f,  0, 0, -1,   0, 1,
			+0.5f, -0.5f, 0.0f,  0, 0, -1,   1, 1,
			+0.5f, +0.5f, 0.0f,  0, 0, -1,   1, 0,
			-0.5f, +0.5f, 0.0f,  0, 0, -1,   0, 0,
		};
		int32_t indices[] = {
			0, 1, 2,
			0, 2, 3,
		};
		defaultMesh->create(vertecies, sizeof(vertecies) / 4, indices, sizeof(indices) / 4);

		if (useFrameBuffer) {
			frameBuffer = std::make_shared<FrameBuffer>();
			frameBuffer->init(resolutionX, resolutionY, {
					{ TextureAttachment::COLOR, color::white},
					{ TextureAttachment::DEPTH},
				}
			);
		}
	}

	void SimpleRenderer::submit(glm::vec3 pos, Mesh* mesh, Texture* texture, Color color) {
		if (!texture) {
			texture = defaultTexture.get();
		}
		if (!mesh) {
			mesh = defaultMesh.get();
		}
		texture->bind(0);
		shader->bind();
		shader->set("uTexture", 0);
		shader->set("uColor", color.vec());
		shader->set("uTransform", glm::translate(glm::mat4(1), pos));
		mesh->vertexArray.submit();
		texture->unbind();
	}

	void SimpleRenderer::begin(const glm::mat4& cameraMatrix, const glm::vec3& lightDirection) {
		if (frameBuffer) {
			frameBuffer->bind();
			frameBuffer->clear();
		}
		shader->set("uProjection", cameraMatrix);
		shader->set("uLightDirection", lightDirection);
	}

	void SimpleRenderer::end() {
		if (frameBuffer) {
			frameBuffer->unbind();
		}
	}

}
