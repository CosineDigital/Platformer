#pragma once

#include <glm/vec2.hpp>


// Tile collider types
enum class TC_Type {

	NONE, // empty / reserved 
	STRENGTH_3, // indestructible tiles
	STRENGTH_2, // hard destructible tiles like stones and question blocks
	STRENGTH_1, // destructible tiles like bricks
	COIN, // collectables
	PIPE // interaction tiles
};

class Entity;
class Player;

class Collider
{
public:

	// same for players
	virtual void resolveEntityCollision(Entity*) noexcept = 0;
	virtual void resolvePlayerCollision(Player*) noexcept = 0;

	TC_Type type;
	glm::vec2 position;
	glm::vec2 dimensions;
};