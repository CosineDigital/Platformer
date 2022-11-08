#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../graphics/renderer.h"
#include "../../core/camera.h"
#include "../json.h"
#include "../../graphics/animator.h"
#include "tile/tile.h"
#include "tile_entity/tile_entity.h"
#include "entity/entity.h"
#include "entity/player.h"
#include "collider/collider.h"
#include "quadtree.h"

class Level
{
public:
    explicit Level();
    ~Level() noexcept;

    Entity* getEntity(uint32_t idx) const noexcept;
    Player* getPlayer(uint32_t idx) const noexcept;
    
    void addPlayer(Player* player) noexcept;
    void addTile(Tile tile, int x, int y) noexcept;
    void addEntity(Entity* entity) noexcept;
    void addTileEntity(TileEntity* entity) noexcept;

    void onMouseEvent(GLFWwindow*, int, int, int) noexcept;
    void onKeyboardEvent(GLFWwindow*, int, int, int, int) noexcept;
    void onScrollEvent(GLFWwindow*, double, double) noexcept;
    void onCursorEvent(GLFWwindow*, double, double) noexcept;

    LineRenderer lineRenderer;

    /**
    * @brief Draw all tiles and entities
    * @param Renderer - Pointer to the current rendering engine
    */
    void draw(Renderer*) noexcept;
    
    /**
    * @brief Draws, updates, and animates all objects in this Scene; pretty cool :)
    */
    void update() noexcept;

    /**
    * Permanently resets / clears the data in the scene
    */
    void reset() noexcept;

    bool play;

    GLFWwindow* parentWindow;

    // Tile data is stored in a large head array
    int width;
    int height;
    Tile* tileData;

    // tile entities (things with functions or animations)
    int tileEntityCount;
    std::vector<TileEntity*> tileEntityData;
    Quadtree<TileEntity>* tileEntityTree;

    // entities
    int entityCount;
    std::vector<Entity*> entityData;
    Quadtree<Entity>* entityTree;

    // Players
    int playerCount;
    std::vector <Player*> players;
};

#endif // SCENE_H_
