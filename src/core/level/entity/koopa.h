#pragma once

#include "entity.h"

// Has two colors and a boolean whther or not it has wings
class Koopa final : public Entity {

public:

	Koopa(glm::vec2 position, bool green, bool winged) {
		this->position = position;
		this->green = green;
		this->winged = winged;
		this->stomped = false;
		this->setCorrectAnimator();
	}

	// If the koopa is green or not
	int green : 1;
	// If the koopa is winged or not
	int winged : 1;
	// whether or not the koopa is spinning
	int spinning : 1;
	bool stomped;
	// if the koopa is on the ground or not
	int touchingGround : 1;
	// the health only needs a value between 0 - 3; 0 = dead; 1 = alive but in cramped position; 2 = alive, no wings; 3 = alive with wings
	int health : 2;

	void draw(Renderer* renderer) noexcept {
		renderer->buffer(position, animator.current());
	}
	
	void update() noexcept {

		position += velocity;
		velocity -= 9.8f / 60.0f;

	}

private:

	void setCorrectAnimator() noexcept {
		if (green) {
			if (winged) { // green and winged
				this->animator = Animator<2>({ Sprites::PARAKOOPA_G_1, Sprites::PARAKOOPA_G_2 }, 15);
			}
			else { // green and not winged
				if (stomped) {
					this->animator = Animator<2>({ Sprites::KOOPA_G_STOMPED_1, Sprites::KOOPA_G_STOMPED_2 }, 15);
				}
				else {
					this->animator = Animator<2>({ Sprites::KOOPA_G_1, Sprites::KOOPA_G_2 }, 15);
				}
			}
		}
		else { // red 
			if (winged) { // red and winged
				this->animator = Animator<2>({ Sprites::PARAKOOPA_R_1, Sprites::PARAKOOPA_R_2 }, 15);
			}
			else { // red and not winged
				if (stomped) {
					this->animator = Animator<2>({ Sprites::KOOPA_R_STOMPED_1, Sprites::KOOPA_R_STOMPED_2 }, 15);
				}
				else {
					this->animator = Animator<2>({ Sprites::KOOPA_R_1, Sprites::KOOPA_R_2 }, 15);
				}
			}
		}
	}

	static constexpr EntityType type_table[4] = {
		EntityType::RED_KOOPA,
		EntityType::GREEN_KOOPA,
		EntityType::RED_PARAKOOPA,
		EntityType::GREEN_PARAKOOPA,
	};

public:

	EntityType getType() const noexcept {
		return type_table[winged * 2 + green];
	}
	
	void reverse() noexcept {

	}
	
	void damage() noexcept {
		if (this->winged) { this->winged = false; }
		
	}
	
	void kill() noexcept {

	}

private:
	Animator<2> animator;
};