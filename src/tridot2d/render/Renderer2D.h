//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Mesh.h"
#include <map>

namespace tridot2d {

	class Renderer2D {
	public:
		std::shared_ptr<FrameBuffer> frameBuffer;
		std::shared_ptr<Texture> circelTexture;

		struct BaseInstance {
		public:
			float depth = 0;
			Color color = color::white;
			Texture* texture = nullptr;
			glm::vec2 coordsTL = { 0, 0 };
			glm::vec2 coordsBR = { 1, 1 };
		};

		struct QuadInstance : public BaseInstance {
		public:
			glm::vec2 position = { 0, 0 };
			glm::vec2 scale = { 1, 1 };
			float rotation = 0;

			Texture* texture2 = nullptr;
			glm::vec2 coordsTL2 = { 0, 0 };
			glm::vec2 coordsBR2 = { 1, 1 };
		};

		struct LineInstance : public BaseInstance {
		public:
			glm::vec2 point1 = { 0, 0 };
			glm::vec2 point2 = { 0, 0 };
			float thickness1 = 1;
			float thickness2 = 1;
			glm::vec2 lastPoint1 = { 0, 0 };
			glm::vec2 lastPoint2 = { 0, 0 };
		};

		enum InstanceType {
			QUAD,
			LINE,
		};

		class Instance {
		public:
			InstanceType type;

			union {
				QuadInstance quad;
				LineInstance line;
			};
		};

		void init();
		void begin(const glm::mat4& projection);
		Instance &submit(InstanceType type = InstanceType::QUAD);
		void end();

		void submitQuad(glm::vec2 pos, glm::vec2 scale, float rotation = 0, float depth = 0, Texture* texture = nullptr, Color color = color::white, const glm::vec2& coords1 = { 0, 0 }, const glm::vec2& coords2 = { 1, 1 });
		void submitCircle(glm::vec2 pos, glm::vec2 scale, float rotation = 0, float depth = 0, Texture* texture = nullptr, Color color = color::white, const glm::vec2& coords1 = { 0, 0 }, const glm::vec2& coords2 = { 1, 1 });
		void submitLine(glm::vec2 p1, glm::vec2 p2, float depth = 0, Color color = color::white, float thickness1 = 1, float thickness2 = 1);

	private:
		class Vertex {
		public:
			glm::vec3 position = { 0, 0, 0 };
			glm::vec4 color = { 0, 0, 0, 0 };
			glm::vec2 texCorrds = { 0, 0 };
			float textureIndex = -1;
			glm::vec2 texCorrds2 = { 0, 0 };
			float textureIndex2 = -1;
		};

		typedef unsigned int Index;

		class Batch {
		public:
			std::vector<Vertex> vertices;
			std::map<Texture*, int> textures;
		};

		std::shared_ptr<Shader> shader;
		std::shared_ptr<VertexArray> mesh;
		std::shared_ptr<Buffer> vertexBuffer;
		std::vector<Instance> instances;
		std::vector<std::shared_ptr<Batch>> batches;
		glm::mat4 projection;

		glm::vec2 lastLinePoint1 = { 0, 0 };
		glm::vec2 lastLinePoint2 = { 0, 0 };
	};

}