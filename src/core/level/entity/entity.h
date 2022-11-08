#ifndef ENTITY_H_
#define ENTITY_H_

#include <glm/vec2.hpp>

#include "../../../graphics/renderer.h"
#include "../../../graphics/line_renderer.h"
#include "../../../graphics/animator.h"
#include "../../../graphics/particle.h"
#include "../collider/collider.h"
#include "../quad.h"

enum class EntityType : size_t {
	NONE,
	// sub enum : enemies
	GOOMBA,
	KOOPA,
	RED_KOOPA,
	GREEN_KOOPA,
	RED_PARAKOOPA,
	GREEN_PARAKOOPA,
	BEETLE,
	LAKITU,
	SPINY,
	PIRHANA_PLANT,
	BOWSER,
	CHEEP_CHEEP,
	BLOOPA,
	POTABOO,
	HAMMER_BRO,
	BULLET_BILL,
	TOAD,
	PEACH,
	// sub enum : items
	RED_MUSHROOM,
	GREEN_MUSHROOM,
	STAR,
	FIRE_FLOWER,
};

class Player;

class Entity {
public:

	// @brief - Other functions
	virtual void draw(Renderer*) noexcept = 0;
	virtual void update() noexcept = 0;

	virtual EntityType getType() const noexcept = 0;
	virtual void reverse() noexcept = 0;

	// @ brief - Damage functions
	virtual void damage() noexcept = 0;
	virtual void kill() noexcept = 0;

	// @brief - Handle collisions with other entities
	virtual void resolvePlayerCollision(Player*) noexcept = 0;
	virtual void resolveEntityCollision(Entity*) noexcept = 0;

	virtual void drawCollider(LineRenderer* lineRenderer) const noexcept {
		Quad q(position, position + dimensions);
		lineRenderer->buffer(q.getBottomLeft(), q.getBottomRight());
		lineRenderer->buffer(q.getTopLeft(), q.getTopRight());
		lineRenderer->buffer(q.getBottomLeft(), q.getTopLeft());
		lineRenderer->buffer(q.getBottomRight(), q.getTopRight());
	}

	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 dimensions;
	EntityType type;
	bool alive;
	bool canJump;

protected:
	/**
	* If this is colliding with another entity e
	*/
	virtual bool colliding(Entity* e) {

		// Check if the two entities collided
		bool x_interception =
			// x_overlaps = (a.left < b.right) && (a.right > b.left)
			(this->position.x < (e->position.x + e->dimensions.x))
			&& (this->position.x + this->dimensions.x > e->position.x);

		bool y_interception =
			// y_overlaps = (a.top < b.bottom) && (a.bottom > b.top)
			(this->position.y + this->dimensions.y < (e->position.y))
			&& (this->position.y > e->position.y + e->dimensions.y);

		return x_interception && y_interception;
	}
};

#endif // !ENTITY_H_

