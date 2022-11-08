#pragma once

#include "quadtree_impl.h"
#include "../../graphics/line_renderer.h"

template<typename T>
class Quadtree final {

public:
	Quadtree(glm::vec2 bottomLeft, glm::vec2 topRight) {
		this->bottomLeft = bottomLeft;
		this->topRight = topRight;
		// This is a heap allocated array; what should the max number be??
		this->count = 0;
		this->buffer = new detail::QuadtreeImpl<T>[1000];
		this->base = create(bottomLeft, topRight);
	}

	~Quadtree() noexcept {
		delete[] buffer;
	}

	void insert(T* t) noexcept {
		this->base->insert(t, this);
	}

	void draw(LineRenderer* lineRenderer) const noexcept {
		// draw every rectangle in the tree
		for (uint32_t i = 0u; i < this->count; i++) {
			this->buffer[i].draw(lineRenderer);
		}
	}

	/**
	* Query for items with the bounds of the quad q
	* @param results - Vector pointer to be filled with pointers to the matching items
	*/
	inline void query(Quad boundary, std::vector<T*>* results) noexcept {
		this->base->query(boundary, results);
	}

	inline void clear() noexcept {
		this->count = 0u; // just reset the count and rebuild the base
		this->base = create(this->bottomLeft, this->topRight);
	}

	// Return pointer to a "new" quadtree
	detail::QuadtreeImpl<T>* create(glm::vec2 bottomLeft, glm::vec2 topRight) noexcept {
		this->buffer[count] = detail::QuadtreeImpl<T>(bottomLeft, topRight); // create in place
		return &this->buffer[count++]; // return the pointer to the newly created quad tree
	}

	uint32_t count;
private:

	// Must be placed in the buffer on construction
	detail::QuadtreeImpl<T>* base;

	// Keeping everything in a buffer is better than reallocating a deleting every time we need to change something...
	detail::QuadtreeImpl<T>* buffer;

	// '.' //
	glm::vec2 bottomLeft;
	glm::vec2 topRight;

};