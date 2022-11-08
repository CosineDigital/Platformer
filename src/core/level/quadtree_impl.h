#pragma once

#include <array>

#include "quad.h"
#include "../../graphics/line_renderer.h"

template<typename T>
class Quadtree;

namespace detail {

	template<typename T>
	class QuadtreeImpl final {

	public:

		QuadtreeImpl() = default;

		/**
		* @tparam T - The base class type to store pointers of
		* @tparam C - The max capacity of any tree
		* @param center - The center of the quad tree, usually half the width and half the height of the level
		* @param dimensions - The dimensions to the edges from the center of the quad tree, usually half the width and half the height of the level
		*/
		QuadtreeImpl(glm::vec2 bottomLeft, glm::vec2 topRight)
		{
			this->divided = false;
			this->count = 0u;
			this->bounds = Quad(bottomLeft, topRight);
			this->childTopLeft = nullptr;
			this->childTopRight = nullptr;
			this->childBottomLeft = nullptr;
			this->childBottomRight = nullptr;
		}

		// No destructor, the deletion of everything is handled in the quadtree class

		/**
		* @brief - Draws the boundaries of the quadtree
		*/
		void draw(LineRenderer* lineRenderer) const noexcept {

			if (!divided) {
				lineRenderer->buffer(bounds.getTopLeft(), bounds.getTopRight()); // top line
				lineRenderer->buffer(bounds.getBottomLeft(), bounds.getBottomRight()); // bottom line
				lineRenderer->buffer(bounds.getBottomLeft(), bounds.getTopLeft()); // left line
				lineRenderer->buffer(bounds.getBottomRight(), bounds.getTopRight()); // right line
			}
			else {
				childTopLeft->draw(lineRenderer);
				childTopRight->draw(lineRenderer);
				childBottomLeft->draw(lineRenderer);
				childBottomRight->draw(lineRenderer);
			}
		}

		void subDivide(Quadtree<T>* buffer) noexcept {
			glm::vec2 center{bounds.getCenter()};
			childTopLeft = buffer->create(glm::vec2{ bounds.bottomLeft.x, center.y }, glm::vec2{ center.x, bounds.topRight.y });
			childTopRight = buffer->create(center, bounds.getTopRight());
			childBottomLeft = buffer->create(bounds.getBottomLeft(), center);
			childBottomRight = buffer->create(glm::vec2{ center.x, bounds.bottomLeft.y }, glm::vec2{ bounds.topRight.x, center.y });
		}

		/**
		* Insert an item into the quadtree, checks whether the point should be going into the quad tree
		* @param t - The item to insert
		*/
		void insert(T* t, Quadtree<T>* buffer) noexcept {

			// First need to test wether the item is with the bounds of the quad
			if (!this->bounds.containsPoint(t->position)) return;

			// Then if it is within the bounds of the quad, we insert it to the data structure
			// If there have only been 4 insertions (0-3) < 4
			// Else, add it to the children if it is within the bounds of their quads
			if (count < 4) {
				this->items[count++] = t;
			}
			// count is greater than 4, at max capacity
			else {
				// If we have not divided, we must or the children trees will be null
				if (!divided) {
					this->subDivide(buffer);
					divided = true;
				}
				childTopLeft->insert(t, buffer);
				childTopRight->insert(t, buffer);
				childBottomLeft->insert(t, buffer);
				childBottomRight->insert(t, buffer);
			}
		}

		/**
		* @breif - Get the items that are within the bounds of the quad q
		* @return - Nothing
		* @param boundary - The boundary to check for items
		* @param results - A pointer to a vector of items; The items found in the tree will be added to this
		*/
		void query(const Quad& boundary, std::vector<T*>* results) const noexcept {

			if (!this->bounds.intersectsQuad(boundary)) { return; }

			for (uint32_t i = 0u; i < count; i++) {
				// if the range is contained
				if (boundary.containsPoint(items[i]->position)) {
					results->push_back(items[i]);
				}
			}

			// If the quadtree has been divided, check each of the children
			if (divided) {
				childTopLeft->query(boundary, results);
				childTopRight->query(boundary, results);
				childBottomLeft->query(boundary, results);
				childBottomRight->query(boundary, results);
			}
		}

	private:
		Quad bounds;

		// See if we can get this into 4 bytes
		uint32_t count;
		std::array<T*, 4> items;

		// Children quadtree pointers, stored in a buffer somewhere
		QuadtreeImpl<T>* childTopLeft;
		QuadtreeImpl<T>* childTopRight;
		QuadtreeImpl<T>* childBottomLeft;
		QuadtreeImpl<T>* childBottomRight;
		bool divided;
	};

}