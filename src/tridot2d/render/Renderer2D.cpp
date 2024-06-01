//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "Renderer2D.h"
#include "RenderContext.h"
#include <algorithm>

static const char* shaderSource = {
#include "shader/2d.glsl"
};

namespace tridot2d {

	void Renderer2D::init(bool unused) {
		shader = std::make_shared<Shader>();
		circelTexture = std::make_shared<Texture>();
		mesh = std::make_shared<VertexArray>();
		vertexBuffer = std::make_shared<Buffer>();

		shader->loadFromSource(shaderSource);

		Image circel;
		circel.init(256, 256, 4, 8);
		for (int x = 0; x < circel.getWidth(); x++) {
			for (int y = 0; y < circel.getHeight(); y++) {
				glm::vec2 p((float)x / circel.getWidth(), (float)y / circel.getHeight());
				p -= 0.5f;
				p *= 2.0f;

				if (glm::length(p) <= 1.0) {
					circel.set(x, y, color::white);
				}
				else {
					circel.set(x, y, color::transparent);
				}
			}
		}
		circelTexture->load(circel);

		vertexBuffer->init(nullptr, 0, sizeof(Vertex), BufferType::VERTEX_BUFFER, true);
		mesh->addVertexBuffer(vertexBuffer, {
			{Type::FLOAT, 3},
			{Type::FLOAT, 4},
			{Type::FLOAT, 2},
			{Type::FLOAT, 1},
			{Type::FLOAT, 2},
			{Type::FLOAT, 1},
		});
	}

	void Renderer2D::begin(const glm::mat4& projection) {
		this->projection = projection;
		lastLinePoint1 = { 0, 0 };
		lastLinePoint2 = { 0, 0 };
	}

	Renderer2D::Instance &Renderer2D::submit(InstanceType type) {
		instances.push_back({});
		Instance& i = instances.back();
		i.type = type;

		switch (type)
		{
		case InstanceType::QUAD:
			i.quad = QuadInstance();
			break;
		case InstanceType::LINE:
			i.line = LineInstance();
			break;
		default:
			break;
		}

		return i;
	}

	void Renderer2D::submitQuad(glm::vec2 pos, glm::vec2 scale, float rotation, float depth, Texture* texture, Color color, const glm::vec2& coords1, const glm::vec2& coords2) {
		QuadInstance& i = submit(InstanceType::QUAD).quad;
		i.position = pos;
		i.scale = scale;
		i.rotation = rotation;
		i.depth = depth;
		i.color = color;
		i.texture = texture;
		i.coordsTL = coords1;
		i.coordsBR = coords2;
	}

	void Renderer2D::submitCircle(glm::vec2 pos, glm::vec2 scale, float rotation, float depth, Texture* texture, Color color, const glm::vec2& coords1, const glm::vec2& coords2) {
		QuadInstance& i = submit(InstanceType::QUAD).quad;
		i.position = pos;
		i.scale = scale;
		i.rotation = rotation;
		i.depth = depth;
		i.color = color;
		i.texture = texture;
		i.coordsTL = coords1;
		i.coordsBR = coords2;
		i.texture2 = circelTexture.get();
	}

	void Renderer2D::submitLine(glm::vec2 p1, glm::vec2 p2, float depth, Color color , float thickness1, float thickness2) {
		LineInstance& i = submit(InstanceType::LINE).line;
		i.point1 = p1;
		i.point2 = p2;
		i.depth = depth;
		i.color = color;
		i.thickness1 = thickness1;
		i.thickness2 = thickness2;
		i.lastPoint1 = lastLinePoint1;
		i.lastPoint2 = lastLinePoint2;
		lastLinePoint1 = p1;
		lastLinePoint2 = p2;
	}

	void Renderer2D::end() {
		std::sort(instances.begin(), instances.end(), [](Instance& a, Instance& b) {
			return a.quad.depth > b.quad.depth;
		});

		FrameBuffer::unbind();
		RenderContext::setBlend(true);
		RenderContext::setDepth(false);

		if (batches.size() == 0) {
			batches.push_back(std::make_shared<Batch>());
		}
		Batch* batch = batches[0].get();

		for (auto& it : instances) {
			BaseInstance& i = it.quad;

			Vertex v1;
			Vertex v2;
			Vertex v3;
			Vertex v4;

			v1.color = i.color.vec();
			v2.color = i.color.vec();
			v3.color = i.color.vec();
			v4.color = i.color.vec();

			if (i.texture != nullptr) {
				if (!batch->textures.contains(i.texture)) {
					batch->textures[i.texture] = batch->textures.size();
				}
				float textureIndex = (float)batch->textures[i.texture];
				v1.textureIndex = textureIndex;
				v2.textureIndex = textureIndex;
				v3.textureIndex = textureIndex;
				v4.textureIndex = textureIndex;

				v1.texCorrds = glm::vec2(i.coordsTL.x, i.coordsBR.y);
				v2.texCorrds = glm::vec2(i.coordsBR.x, i.coordsBR.y);
				v3.texCorrds = glm::vec2(i.coordsBR.x, i.coordsTL.y);
				v4.texCorrds = glm::vec2(i.coordsTL.x, i.coordsTL.y);
			}

			switch (it.type)
			{
			case InstanceType::QUAD: {
				auto& i = it.quad;

				i.scale *= 0.5f;
				v1.position = glm::vec3(-i.scale.x, -i.scale.y, 0);
				v2.position = glm::vec3(+i.scale.x, -i.scale.y, 0);
				v3.position = glm::vec3(+i.scale.x, +i.scale.y, 0);
				v4.position = glm::vec3(-i.scale.x, +i.scale.y, 0);

				if (i.rotation != 0) {
					float sin = glm::sin(i.rotation);
					float cos = glm::cos(i.rotation);
					v1.position = glm::vec3(v1.position.x * cos - v1.position.y * sin, v1.position.x * sin + v1.position.y * cos, v1.position.z);
					v2.position = glm::vec3(v2.position.x * cos - v2.position.y * sin, v2.position.x * sin + v2.position.y * cos, v2.position.z);
					v3.position = glm::vec3(v3.position.x * cos - v3.position.y * sin, v3.position.x * sin + v3.position.y * cos, v3.position.z);
					v4.position = glm::vec3(v4.position.x * cos - v4.position.y * sin, v4.position.x * sin + v4.position.y * cos, v4.position.z);
				}

				v1.position += glm::vec3(i.position, i.depth * 0.01);
				v2.position += glm::vec3(i.position, i.depth * 0.01);
				v3.position += glm::vec3(i.position, i.depth * 0.01);
				v4.position += glm::vec3(i.position, i.depth * 0.01);


				if(i.texture2 != nullptr) {
					if (!batch->textures.contains(i.texture2)) {
						batch->textures[i.texture2] = batch->textures.size();
					}
					float textureIndex = (float)batch->textures[i.texture2];
					v1.textureIndex2 = textureIndex;
					v2.textureIndex2 = textureIndex;
					v3.textureIndex2 = textureIndex;
					v4.textureIndex2 = textureIndex;


					v1.texCorrds2 = glm::vec2(i.coordsTL2.x, i.coordsBR2.y);
					v2.texCorrds2 = glm::vec2(i.coordsBR2.x, i.coordsBR2.y);
					v3.texCorrds2 = glm::vec2(i.coordsBR2.x, i.coordsTL2.y);
					v4.texCorrds2 = glm::vec2(i.coordsTL2.x, i.coordsTL2.y);
				}

				break;
			}
			case InstanceType::LINE: {
				auto& i = it.line;

				glm::vec2 diff = i.point1 - i.point2;
				glm::vec2 dir = glm::normalize(diff);
				glm::vec2 normal1 = { -dir.y, dir.x };
				glm::vec2 normal2 = { -dir.y, dir.x };

				if (i.point1 == i.lastPoint2) {
					glm::vec2 diff = i.lastPoint1 - i.lastPoint2;
					glm::vec2 dir = glm::normalize(diff);
					normal1 = { -dir.y, dir.x };
				}
				if (i.point2 == i.lastPoint1) {
					glm::vec2 diff = i.lastPoint1 - i.lastPoint2;
					glm::vec2 dir = glm::normalize(diff);
					normal2 = { -dir.y, dir.x };
				}

				v1.position = glm::vec3(i.point1 + normal1 * i.thickness1 * 0.5f, i.depth * 0.01);
				v2.position = glm::vec3(i.point2 + normal2 * i.thickness2 * 0.5f, i.depth * 0.01);
				v3.position = glm::vec3(i.point2 - normal2 * i.thickness2 * 0.5f, i.depth * 0.01);
				v4.position = glm::vec3(i.point1 - normal1 * i.thickness1 * 0.5f, i.depth * 0.01);

				break;
			}
			default:
				break;
			}

			batch->vertices.push_back(v1);
			batch->vertices.push_back(v2);
			batch->vertices.push_back(v3);
			batch->vertices.push_back(v1);
			batch->vertices.push_back(v3);
			batch->vertices.push_back(v4);
		}

		shader->bind();
		shader->set("uProjection", projection);

		std::vector<int> ids;
		for (int i = 0; i < batch->textures.size(); i++) {
			ids.push_back(i);
		}
		shader->set("uTextures", ids.data(), ids.size());
		
		for (auto i : batch->textures) {
			i.first->bind(i.second);
		}

		vertexBuffer->setData(batch->vertices.data(), batch->vertices.size() * sizeof(Vertex), 0);
		mesh->submit(batch->vertices.size());

		for (auto i : batch->textures) {
			i.first->unbind();
		}

		instances.clear();
		batch->vertices.clear();
		batch->textures.clear();
	}

}
