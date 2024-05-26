//
// Copyright (c) 2024 Julian Hinxlage. All rights reserved.
//

#include "BroadPhase.h"
#include "PhysicsSystem.h"

namespace tridot2d {
    
    void EachBroadPhase::each(const std::function<void(Body*, Body*)>& callback) {
        int count = physics->getBodyCount();
        for (int i = 0; i < count; i++) {
            Body *a = physics->getBody(i);
            for (int j = i+1; j < count; j++) {
                Body* b = physics->getBody(j);
                callback(a, b);
            }
        }
    }

	StaticGridBroadPhase::StaticGridBroadPhase(const glm::vec2& cellSize, int cellCountX, int cellCountY) {
		init(cellSize, cellCountX, cellCountY);
	}

	void StaticGridBroadPhase::init(const glm::vec2& cellSize, int cellCountX, int cellCountY) {
		cells.clear();
		cellByBodyIndex.clear();
		outsideCell = Cell();
		this->cellCountX = cellCountX;
		this->cellCountY = cellCountY;
		this->cellSize = cellSize;

		offset = -cellSize * 0.5f * glm::vec2(cellCountX, cellCountY);
		cells.resize(cellCountX * cellCountY);
	}

	void StaticGridBroadPhase::clearBodies() {
		for (auto& cell : cells) {
			cell.bodies.clear();
		}
		outsideCell.bodies.clear();
		cellByBodyIndex.clear();
	}

	void StaticGridBroadPhase::updateBody(Body* body) {
		if (cellByBodyIndex.size() <= body->index) {
			cellByBodyIndex.resize(body->index + 1);
		}
		Cell* current = cellByBodyIndex[body->index];
		Cell* next = getCell(body->position);

		if (current != next) {
			if (current) {
				current->bodies.erase(body);
			}
			next->bodies.insert(body);
			cellByBodyIndex[body->index] = next;
		}
	}

	void StaticGridBroadPhase::removeBody(Body* body) {
		if (cellByBodyIndex.size() <= body->index) {
			return;
		}
		Cell* current = cellByBodyIndex[body->index];
		if (current) {
			current->bodies.erase(body);
		}
	}

	void StaticGridBroadPhase::each(const std::function<void(Body*, Body*)>& callback) {
		eachCell(outsideCell, outsideCell, callback);

		for (int x = 0; x < cellCountX; x++) {
			for (int y = 0; y < cellCountY; y++) {
				Cell* a = getCell(offset + cellSize * 0.5f + cellSize * glm::vec2(x, y));

				bool outsideHit = false;
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						Cell* b = getCell(offset + cellSize * 0.5f + cellSize * glm::vec2(x + i, y + j));
						if (b != &outsideCell) {
							eachCell(*a, *b, callback);
						}
						else {
							outsideHit = true;
						}
					}
				}

				if (outsideHit) {
					eachCell(*a, outsideCell, callback);
				}
			}
		}

	}

	StaticGridBroadPhase::Cell* StaticGridBroadPhase::getCell(glm::vec2 pos) {
		pos.x -= offset.x;
		pos.y -= offset.y;
		if (pos.x < 0) {
			pos.x -= cellSize.x;
		}
		if (pos.y < 0) {
			pos.y -= cellSize.y;
		}

		int x = pos.x / cellSize.x;
		int y = pos.y / cellSize.y;

		if (x >= 0 && x < cellCountX) {
			if (y >= 0 && y < cellCountY) {
				return &cells[x + y * cellCountX];
			}
		}

		return &outsideCell;
	}

	void StaticGridBroadPhase::eachCell(Cell& a, Cell& b, const std::function<void(Body*, Body*)>& callback) {
		if (&a == &b) {
			for (auto i = a.bodies.begin(); i != a.bodies.end(); i++) {
				auto j = i;
				j++;
				for (; j != a.bodies.end(); j++) {
					callback(*i, *j);
				}
			}
		}
		else {
			for (auto& i : a.bodies) {
				for (auto& j : b.bodies) {
					callback(i, j);
				}
			}
		}
	}

}
