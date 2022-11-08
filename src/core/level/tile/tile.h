#pragma once

struct Tile {
	Tile(bool solid, int sprite) : mSolid(solid), mSprite(sprite) {}
	Tile() : mSolid(false), mSprite(0) {}

	bool mSolid;
	int mSprite;
};