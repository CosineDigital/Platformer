#pragma once

#include <glm/vec2.hpp>

struct Quad {

	Quad() = default;

	Quad(glm::vec2 bottomLeft, glm::vec2 topRight) {
		this->bottomLeft = bottomLeft;
		this->topRight = topRight;
	}

	glm::vec2 bottomLeft;
	glm::vec2 topRight;

	inline float width() const noexcept {
		return topRight.x - bottomLeft.x;
	}

	inline float height() const noexcept {
		return topRight.y - bottomLeft.y;
	}

	inline glm::vec2 getTopLeft() const noexcept {
		return { topRight - glm::vec2{ width(), 0.0f } };
	}

	inline glm::vec2 getTopRight() const noexcept { 
		return topRight; 
	}

	inline glm::vec2 getBottomLeft() const noexcept {
		return bottomLeft;
	}

	inline glm::vec2 getBottomRight() const noexcept {
		return { bottomLeft + glm::vec2{ width(), 0.0f } };
	}

	inline glm::vec2 getCenter() const noexcept {
		return { bottomLeft.x + width() / 2.0f, bottomLeft.y + height() / 2.0f };
	}

	inline bool containsQuad(const Quad& other) const noexcept {
		return containsPoint(other.bottomLeft) && containsPoint(other.topRight);
	}

	inline bool containsPoint(const glm::vec2& point) const noexcept {
		// to be contained in a rect, the point must be greater than or equal to the bottom left corner
		return ((point.x >= this->bottomLeft.x) && (point.y >= this->bottomLeft.y)) &&
			// and less than the top right corner
			((point.x < this->topRight.x) && (point.y < topRight.y));
	}

	inline bool intersectsQuad(const Quad& other) const noexcept {

		return !(other.getBottomLeft().x > this->getTopRight().x ||
			other.getTopRight().x < this->getBottomLeft().x ||
			other.getBottomLeft().y > this->getTopRight().y ||
			other.getTopRight().y < this->getBottomLeft().y);

	}
};
