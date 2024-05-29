
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

		void init(bool useFrameBuffer, int resolutionX = 0, int resolutionY = 0);
		void submitQuad(glm::vec2 pos, glm::vec2 scale, float rotation = 0, float depth = 0, Texture* texture = nullptr, Color color = color::white, const glm::vec2 &coords1 = {0, 0}, const glm::vec2 &coords2 = {1, 1});
		void submitCircle(glm::vec2 pos, glm::vec2 scale, float rotation = 0, float depth = 0, Texture* texture = nullptr, Color color = color::white, const glm::vec2& coords1 = { 0, 0 }, const glm::vec2& coords2 = { 1, 1 });
		void submitLine(glm::vec2 p1, glm::vec2 p2, float depth = 0, Color color = color::white, float thickness1 = 1, float thickness2 = 1, bool pixelScale = true);
		void begin(const glm::mat4& cameraMatrix = glm::mat4(1), bool clear = true);
		void end();

	private:
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> blankTexture;
		std::shared_ptr<Mesh> quadMesh;
		std::shared_ptr<Buffer> instanceBuffer;

		std::shared_ptr<Shader> lineShader;
		std::shared_ptr<VertexArray> lineMesh;
		std::shared_ptr<Buffer> lineBuffer;

		glm::mat4 cameraMatrix;
		glm::vec2 resoultion;
		glm::vec2 lastLinePoint1;
		glm::vec2 lastLinePoint2;

		class Instance {
		public:
			glm::mat4 transform;
			glm::vec4 color;
			glm::vec2 coords1;
			glm::vec2 coords2;
		};
		class Batch {
		public:
			std::vector<Instance> instances;
		};

		class LineVertex {
		public:
			glm::vec3 pos;
			glm::vec4 color;
		};
		class LineBatch {
		public:
			std::vector<LineVertex> vertices;
		};

		std::map<Texture*, Batch> quadBatches;
		std::map<Texture*, Batch> circleBatches;
		LineBatch lineBatch;
	};

}