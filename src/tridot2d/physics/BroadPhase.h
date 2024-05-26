//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#pragma once

#include "Shape.h"
#include <functional>
#include <set>

namespace tridot2d {

	class BroadPhase {
	public:
		class PhysicsSystem* physics = nullptr;

		virtual void clearBodies() {};
		virtual void updateBody(Body* body) {};
		virtual void removeBody(Body* body) {};
		virtual void each(const std::function<void(Body*, Body*)>& callback) {};
	};

	class EachBroadPhase : public BroadPhase{
	public:
		void each(const std::function<void(Body*, Body*)>& callback) override;
	};

	class StaticGridBroadPhase : public BroadPhase {
	public:

		StaticGridBroadPhase(const glm::vec2& cellSize, int cellCountX, int cellCountY);

		void init(const glm::vec2& cellSize, int cellCountX, int cellCountY);

		void clearBodies() override;
		void updateBody(Body* body) override;
		void removeBody(Body* body) override;
		void each(const std::function<void(Body*, Body*)>& callback) override;

	private:
		class Cell {
		public:
			std::set<Body*> bodies;
		};
		std::vector<Cell> cells;
		std::vector<Cell*> cellByBodyIndex;
		int cellCountX = 0;
		int cellCountY = 0;
		glm::vec2 cellSize = { 0, 0 };
		glm::vec2 offset = { 0, 0 };
		Cell outsideCell;

		Cell* getCell(glm::vec2 pos);
		void eachCell(Cell &a, Cell &b, const std::function<void(Body*, Body*)>& callback);
	};

}
