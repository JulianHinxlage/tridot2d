//
// Copyright (c) 2025 Julian Hinxlage. All rights reserved.
//

#pragma once

#include <vector>
#include <set>
#include <glm/glm.hpp>

namespace tridot2d {

	template<typename T>
	class GridT {
	public:
		int countX = 0;
		int countY = 0;
		glm::vec2 offset = { 0, 0 };
		glm::vec2 cellSize = { 1, 1 };
		std::vector<T> cells;
		T outside;

		GridT() {}

		void init(glm::vec2 offset, glm::vec2 cellSize, int countX = 0, int countY = 0) {
			this->offset = offset;
			this->cellSize = cellSize;
			this->countX = countX;
			this->countY = countY;
			cells.clear();
			cells.resize(countY * countX);
		}

		void resizeToFit(glm::vec2 position) {
			if (position.x < offset.x) {
				position.x -= cellSize.x;
			}
			if (position.y < offset.y) {
				position.y -= cellSize.x;
			}
			int indexX = (position.x - offset.x) / cellSize.x;
			int indexY = (position.y - offset.y) / cellSize.y;
			
			if (indexX < 0 || indexX >= countX || indexY < 0 || indexY >= countY) {
				std::vector<T> oldCells;
				oldCells.swap(cells);
				
				int newCountX = countX;
				int newCountY = countY;
				int offsetX = 0;
				int offsetY = 0;

				if (indexX >= countX) {
					newCountX = indexX + 1;
				}
				if (indexY >= countY) {
					newCountY = indexY + 1;
				}
				if (indexX < 0) {
					newCountX = countX - indexX;
					offsetX = -indexX;
				}
				if (indexY < 0) {
					newCountY = countY - indexY;
					offsetY = -indexY;
				}

				cells.resize(newCountY * newCountX);
				for (int y = 0; y < newCountY; y++) {
					for (int x = 0; x < newCountX; x++) {
						int oldX = x + offsetX;
						int oldY = y + offsetY;
						int index = y * newCountX + x;
						if (oldY >= 0 && oldY < countY) {
							if (oldX >= 0 && oldX < countX) {
								int oldIndex = oldY * countX + oldX;
								cells[index] = oldCells[oldIndex];
							}
						}
					}
				}

				countX = newCountX;
				countY = newCountY;
				offset -= cellSize * glm::vec2(offsetX, offsetY);
			}
		}

		glm::vec2 getCellSize() {
			return cellSize;
		}

		glm::vec2 getPosition(glm::vec2 position) {
			if (position.x < offset.x) {
				position.x -= cellSize.x;
			}
			if (position.y < offset.y) {
				position.y -= cellSize.y;
			}
			int indexX = (position.x - offset.x) / cellSize.x;
			int indexY = (position.y - offset.y) / cellSize.y;
			return offset + cellSize * glm::vec2(indexX, indexY) + cellSize / 2.0f;
		}

		T& get(glm::vec2 position) {
			int index = getIndex(position);
			if (index >= 0 && index < cells.size()) {
				return cells[index];
			}
			else {
				return outside;
			}
		}

		void set(glm::vec2 position, const T& value) {
			int index = getIndex(position);
			if (index >= 0 && index < cells.size()) {
				cells[index] = value;
			}
		}

		bool update(glm::vec2 position, const T& value, glm::vec2 prevPosition) {
			int prevIndex = getIndex(prevPosition);
			int index = getIndex(position);

			if (prevIndex != index) {
				if (prevIndex >= 0 && prevIndex < cells.size()) {
					cells[prevIndex] = T();
				}

				if (index >= 0 && index < cells.size()) {
					cells[index] = value;
					return true;
				}
				return true;
			}
			else {
				return false;
			}
		}

		void reset(glm::vec2 position, const T& value) {
			int index = getIndex(position);
			if (index >= 0 && index < cells.size()) {
				cells[index] = T();
			}
		}

		int getIndex(glm::vec2 position) {
			if (position.x < offset.x) {
				return -1;
			}
			if (position.y < offset.y) {
				return -1;
			}
			int indexX = (position.x - offset.x) / cellSize.x;
			int indexY = (position.y - offset.y) / cellSize.y;
			if (indexX >= countX) {
				return -1;
			}
			if (indexY >= countY) {
				return -1;
			}
			return indexY * countX + indexX;
		}
	};

	template<typename T>
	class MultiGridT : public GridT<std::set<T>> {
	public:
		void set(glm::vec2 position, const T& value) {
			int index = this->getIndex(position);
			if (index >= 0 && index < this->cells.size()) {
				this->cells[index].insert(value);
			}
		}

		bool update(glm::vec2 position, const T& value, glm::vec2 prevPosition) {
			int prevIndex = this->getIndex(prevPosition);
			int index = this->getIndex(position);

			if (prevIndex != index) {
				if (prevIndex >= 0 && prevIndex < this->cells.size()) {
					this->cells[prevIndex].erase(value);
				}

				if (index >= 0 && index < this->cells.size()) {
					this->cells[index].insert(value);
					return true;
				}
				return true;
			}
			else {
				return false;
			}
		}

		void reset(glm::vec2 position, const T& value) {
			int index = this->getIndex(position);
			if (index >= 0 && index < this->cells.size()) {
				this->cells[index].erase(value);
			}
		}
	};

}
