//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Renderer2D.h"
#include "RenderContext.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

static const char* shaderSource =
{
#include "shader/2d.glsl"
};

static const char* lineShaderSource =
{
#include "shader/line.glsl"
};

namespace tridot2d {

	void Renderer2D::init(bool useFrameBuffer, int resolutionX, int resolutionY) {
		shader = std::make_shared<Shader>();
		shader->loadFromSource(shaderSource);

		blankTexture = std::make_shared<Texture>();
		blankTexture->create(1, 1);
		Color color = color::white;
		blankTexture->setData(&color, 1);

		quadMesh = std::make_shared<Mesh>();
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
		quadMesh->create(vertecies, sizeof(vertecies) / 4, indices, sizeof(indices) / 4);

		instanceBuffer = std::make_shared<Buffer>();
		instanceBuffer->init(nullptr, 0, 1, BufferType::VERTEX_BUFFER, true);
		quadMesh->vertexArray.addVertexBuffer(instanceBuffer, {
			{Type::FLOAT, 4},
			{Type::FLOAT, 4},
			{Type::FLOAT, 4},
			{Type::FLOAT, 4},
			{Type::FLOAT, 4},
			{Type::FLOAT, 2},
			{Type::FLOAT, 2},
		}, 1);

		if (useFrameBuffer) {
			frameBuffer = std::make_shared<FrameBuffer>();
			frameBuffer->init(resolutionX, resolutionY, {
					{ TextureAttachment::COLOR, color::white},
					{ TextureAttachment::DEPTH},
				}
			);
		}

		lineShader = std::make_shared<Shader>();
		lineShader->loadFromSource(lineShaderSource);
		lineMesh = std::make_shared<VertexArray>();
		lineBuffer = std::make_shared<Buffer>();
		lineBuffer->init(nullptr, 0, sizeof(LineVertex), BufferType::VERTEX_BUFFER, true);
		lineMesh->addVertexBuffer(lineBuffer, {
			{Type::FLOAT, 3},
			{Type::FLOAT, 4},
		});

		RenderContext::setBlend(true);
		RenderContext::setDepth(true);
	}

	void Renderer2D::submitQuad(glm::vec2 pos, glm::vec2 scale, float rotation, float depth, Texture* texture, Color color, const glm::vec2& coords1, const glm::vec2& coords2) {
		glm::mat4 transform(1);
		transform = glm::translate(transform, glm::vec3(pos, depth * 0.01));
		transform = glm::rotate(transform, rotation, glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(scale, 0));

		Instance instance;
		instance.transform = transform;
		instance.color = color.vec();
		instance.coords1 = coords1;
		instance.coords2 = coords2;
		if (!texture) {
			texture = blankTexture.get();
		}
		quadBatches[texture].instances.push_back(instance);
	}

	void Renderer2D::submitCircle(glm::vec2 pos, glm::vec2 scale, float rotation, float depth, Texture* texture, Color color, const glm::vec2& coords1, const glm::vec2& coords2) {
		glm::mat4 transform(1);
		transform = glm::translate(transform, glm::vec3(pos, depth * 0.01));
		transform = glm::rotate(transform, rotation, glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(scale, 0));

		Instance instance;
		instance.transform = transform;
		instance.color = color.vec();
		instance.coords1 = coords1;
		instance.coords2 = coords2;
		if (!texture) {
			texture = blankTexture.get();
		}
		circleBatches[texture].instances.push_back(instance);
	}

	void Renderer2D::submitLine(glm::vec2 p1, glm::vec2 p2, float depth, Color color, float thickness1, float thickness2, bool pixelScale) {
		glm::vec2 diff = p1 - p2;
		glm::vec2 dir = glm::normalize(diff);
		glm::vec3 normal1 = { -dir.y, dir.x, 0 };
		glm::vec3 normal2 = { -dir.y, dir.x, 0 };

		if (p1 == lastLinePoint2) {
			glm::vec2 diff = lastLinePoint1 - lastLinePoint2;
			glm::vec2 dir = glm::normalize(diff);
			normal1 = { -dir.y, dir.x, 0 };
		}

		lastLinePoint1 = p1;
		lastLinePoint2 = p2;
		p1 = cameraMatrix * glm::vec4(p1, 0, 1);
		p2 = cameraMatrix * glm::vec4(p2, 0, 1);
		normal1 = cameraMatrix * glm::vec4(normal1, 0);
		normal2 = cameraMatrix * glm::vec4(normal2, 0);

		LineVertex v1;
		LineVertex v2;
		LineVertex v3;
		LineVertex v4;

		v1.color = color.vec();
		v2.color = v1.color;
		v3.color = v1.color;
		v4.color = v1.color;

		glm::vec2 scale = { 0.5f, 0.5f };
		if (pixelScale) {
			scale = { 1.0f / resoultion.y / cameraMatrix[1][1], 1.0f / resoultion.y / cameraMatrix[1][1] };
		}

		v1.pos = glm::vec3(p1, depth * 0.01) + normal1 * thickness1 * glm::vec3(scale, 1);
		v2.pos = glm::vec3(p2, depth * 0.01) + normal2 * thickness2 * glm::vec3(scale, 1);
		v4.pos = glm::vec3(p1, depth * 0.01) - normal1 * thickness1 * glm::vec3(scale, 1);
		v3.pos = glm::vec3(p2, depth * 0.01) - normal2 * thickness2 * glm::vec3(scale, 1);

		lineBatch.vertices.push_back(v1);
		lineBatch.vertices.push_back(v2);
		lineBatch.vertices.push_back(v3);
		lineBatch.vertices.push_back(v1);
		lineBatch.vertices.push_back(v3);
		lineBatch.vertices.push_back(v4);
	}

	void Renderer2D::begin(const glm::mat4& cameraMatrix, bool clear, bool enableDepth) {

		if (enableDepth) {
			RenderContext::setDepth(true);
		}
		else {
			RenderContext::setDepth(false);
		}

		if (frameBuffer) {
			frameBuffer->bind();
			if (clear) {
				frameBuffer->clear();
			}
		}
		else {
			FrameBuffer::unbind();
		}
		this->cameraMatrix = cameraMatrix;
		shader->bind();
		shader->set("uProjection", cameraMatrix);

		int x = 0;
		int y = 0;
		glfwGetFramebufferSize((GLFWwindow*)RenderContext::get(), &x, &y);
		resoultion = { x, y };

		lastLinePoint1 = { 0, 0 };
		lastLinePoint2 = { 0, 0 };
	}

	void Renderer2D::end() {
		for (auto& i : circleBatches) {
			shader->bind();
			shader->set("uSize", 1.0f);
			instanceBuffer->setData(i.second.instances.data(), i.second.instances.size() * sizeof(Instance), 0);
			i.first->bind(0);
			shader->set("uTexture", 0);
			quadMesh->vertexArray.submit(-1, i.second.instances.size());
			i.second.instances.clear();
			i.first->unbind();
		}
		circleBatches.clear();
		for (auto& i : quadBatches) {
			shader->bind();
			instanceBuffer->setData(i.second.instances.data(), i.second.instances.size() * sizeof(Instance), 0);
			shader->set("uSize", 4.0f);
			i.first->bind(0);
			shader->set("uTexture", 0);
			quadMesh->vertexArray.submit(-1, i.second.instances.size());
			i.second.instances.clear();
			i.first->unbind();
		}
		quadBatches.clear();

		if(lineBatch.vertices.size() > 0) {
			lineShader->bind();
			lineShader->set("uProjection", glm::mat4(1));
			lineBuffer->setData(lineBatch.vertices.data(), lineBatch.vertices.size() * sizeof(LineVertex));
			lineMesh->submit(lineBatch.vertices.size());
			lineBatch.vertices.clear();
		}

		if (frameBuffer) {
			frameBuffer->unbind();
		}
	}

}
