#pragma once

#include "entity.h"

class Player final : public Entity {

public:

	Player() {
		mCamera = new Camera();
	}

	~Player() {
		delete mCamera;
	}
	
	void draw(Renderer* renderer) noexcept {
		if (alive) {
			//renderer->buffer(this->position, Sprites::BRICK_BOT);
		}
	}

	void handleInput(GLFWwindow* window) {

		// handle each button
		GLFWgamepadstate state;
		glfwGetGamepadState(0, &state);
		
		if (state.buttons[GLFW_GAMEPAD_BUTTON_A] || state.buttons[GLFW_GAMEPAD_BUTTON_B]) {
			// jump if the player is touching the ground -> add upwards velocity

		}

		if (state.buttons[GLFW_GAMEPAD_BUTTON_X] || state.buttons[GLFW_GAMEPAD_BUTTON_Y]) {
			
		}

		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]) {
			// set the direction
			this->position.x += 1.0f / 60.0f;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]) {
			// set the direction
			this->position.x -= 1.0f / 60.0f;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]) {
			// set the direction
			this->position.y += 1.0f / 60.0f;
		}
		if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]) {
			// set the direction
			this->position.y -= 1.0f / 60.0f;
		}
	}

	virtual void update() noexcept {

		// TODO: implement
		position += velocity;
		
	}

	EntityType getType() const noexcept {
		return EntityType::PIRHANA_PLANT;
	}

	void reverse() noexcept {
		// does nothing...
	}

	void damage() noexcept {
		// decrement health
			
	}

	void kill() noexcept {
		alive = false;
		// create a particle of player falling off the screen

	}

	void resolvePlayerCollision(Player*) noexcept {
		// no collisions between players
	}

	void resolveEntityCollision(Entity*) noexcept {
		// no collisions with entities, the entities will handle all collisions
	}

	Camera* getCamera() {
		return mCamera;
	}

private:
	GLFWgamepadstate state;
	Camera* mCamera;

	enum class PowerUpState : size_t {
		// alive, but already part of entity
		SMALL,
		LARGE,
		FIRE,
		INVINCIBLE
	};
};