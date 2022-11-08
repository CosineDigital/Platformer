#include "level.h"

#define MIN_LEVEL_WIDTH 100u * 2u;
#define MIN_LEVEL_HEIGHT 13u * 2u;

Level::Level() : play(false) {

    // default dimensions
	this->width = MIN_LEVEL_WIDTH;
	this->height = MIN_LEVEL_HEIGHT;

    // Setup the data width * height
    this->tileData = new Tile[this->width * this->height];

    this->tileEntityCount = 0u;
    // TODO: fix
    //this->tileEntityTree = new QuadTree<TileEntity>({ 0.0f,0.0f }, { 100.0f, 13.0f });

    this->entityCount = 0u;
    this->entityTree = new Quadtree<Entity>({ 0.0f,0.0f }, { 100.0f, 13.0f });

    this->playerCount = 0u;
    this->addPlayer(new Player());
}

Level::~Level() noexcept {

    // Delete entity data
    for (int i = 0; i < entityCount; i++) {
        delete entityData[i];
    }

    for (int i = 0; i < playerCount; i++) {
        delete players[i];
    }

    delete entityTree;
    delete[] tileData;
}

Player* Level::getPlayer(uint32_t idx) const noexcept {

    return this->players[idx];
}

Entity* Level::getEntity(uint32_t idx) const noexcept {

    return this->entityData[idx];
}

void Level::addPlayer(Player* player) noexcept
{
    players.push_back(player);
    entityTree->insert(player);
    playerCount++;
}


void Level::addTile(Tile tile, int x, int y) noexcept {

    this->tileData[x + y * this->width] = tile;
}

void Level::addTileEntity(TileEntity* tileEntity) noexcept {

    this->tileEntityData.push_back(tileEntity);
    this->tileEntityTree->insert(tileEntity);
    this->tileEntityCount++;
}

void Level::onMouseEvent(GLFWwindow*, int, int, int) noexcept
{
}

void Level::onKeyboardEvent(GLFWwindow*, int, int, int, int) noexcept
{
}

void Level::onScrollEvent(GLFWwindow*, double, double) noexcept
{
}

void Level::onCursorEvent(GLFWwindow*, double, double) noexcept
{
}

void Level::addEntity(Entity* entity) noexcept {

    this->entityData.push_back(entity);
    this->entityTree->insert(entity);
    this->entityCount++;
}




void Level::draw(Renderer* renderer) noexcept {

    renderer->clear();

    // First draw the tiles, since they are the background, we buffer these first
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            Tile* tile = &this->tileData[x + y * this->width];
            if (tile->mSprite != 0) {
                renderer->buffer({ x, y }, tile->mSprite);
            }
        }
    }


    entityTree->clear();
    for (int i = 0; i < entityCount; i++) {

        entityTree->insert(entityData[i]);
    }


    
    

    // Draw every player in the level and handle input from gamepads
    for (int p = 0; p < playerCount; p++) {
        players[p]->draw(renderer);
        players[p]->handleInput(this->parentWindow);
    }


    // Draw every entity (including players) in the level and check for collisions -> update appropriately
    for (int i = 0; i < entityCount + playerCount; i++) {

        Entity* e{ nullptr };
        if (i < entityCount) {
            e = getEntity(i);
            if (!e->alive) { continue; }
            e->draw(renderer);

            // check for entity collisions with the players
            for (int p = 0u; p < playerCount; p++) {
                e->resolvePlayerCollision(getPlayer(p));
            }


            if (play) {
                e->update();
            }
        }
        else {
            // check collisions with world with the players
            e = getPlayer(i - entityCount);
        }


        // Resolve entity collisions with other entities
        if (i == 0) {
            std::vector<Entity*> nearbyEntities;
            // Get the entities within the range of one block of the entity's hitbox
            entityTree->query({ e->position - glm::vec2{1.0f, 1.0f}, e->position + glm::vec2{2.0f, 2.0f} }, &nearbyEntities);
            //std::cout << "Nearby to 0: " << nearbyEntities.size() << std::endl;
            for (Entity* e2 : nearbyEntities) {
                e2->resolveEntityCollision(e);
            }
        }

        // Resolve entity collisions with the terrain's colliders
    }
}

void Level::update() noexcept {
}

void Level::reset() noexcept
{
    this->width = MIN_LEVEL_WIDTH;
    this->height = MIN_LEVEL_HEIGHT;

    this->entityCount = 0;
    this->entityData.clear();
}
