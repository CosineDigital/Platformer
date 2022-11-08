#pragma once

#include "entity.h"

class Goomba final : public Entity {

public:
	Goomba(glm::vec2 pos) {
		this->position = pos;
		this->alive = true;
		this->deathDuration = 30u;
		dimensions = { 1.0f, 1.0f };
	}

	void draw(Renderer* renderer) noexcept {
		if (alive) {
			renderer->buffer(this->position, animator.current());
		} 
		else {
			deathDuration--;
		}
	};

	inline EntityType getType() const noexcept {
		return EntityType::GOOMBA;
	}

	void resolvePlayerCollision(Player* p) noexcept {
		
		if (!colliding(p)) { return; }

		// If the player's feet are above the goomba's midsection, it can be stomped
		if (p->position.y > 0.65f * this->dimensions.y + this->position.y) {
			this->kill();
		}
		// else
		else {
			p->damage();
		}
	}

	void resolveEntityCollision(Entity* e) noexcept {

		if (!colliding(e)) return;

		// if they collided...

		// a koopa can kill the goomba if it's spinning (velocity is very large)
		if (e->getType() == EntityType::GREEN_KOOPA) {
			if (e->velocity.x > 2.5f / 60.0f) {
				this->kill();
			}
			else { this->reverse(); e->reverse(); }
		}
		else {
			// if it's neither a player nor spinning koopa, flip the two entities
			this->reverse(); 
			e->reverse();
		}
	}

	void kill() noexcept {
		this->alive = false;
	}

	void update() noexcept {
		this->position += this->velocity;
		animator.update();
	}

	void damage() noexcept {}

	void reverse() noexcept {
		this->velocity *= -1.0f;
	}

private:


	// walking animation, 20 frames in between each
	Animator animator = Animator({Sprites::GOOMBA_LEFT, Sprites::GOOMBA_RIGHT}, 20);

	// the amount of time the goomba stays stomped after dying
	uint32_t deathDuration;
};