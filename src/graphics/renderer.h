#ifndef RENDERER_H_
#define RENDERER_H_

#include <map>

#include "../core/camera.h"
#include "../core/transform.h"
#include "../graphics/shader_program.h"
#include "../graphics/sprite_sheet.h"
#include "../core/transform.h"
#include "../graphics/animator.h"
#include "../graphics/sprite.h"
#include "../graphics/vertex.h"
#include "../core/camera.h"
#include "../core/json.h"

class Renderer
{
public:
    friend class Editor;

    explicit Renderer();
    ~Renderer() noexcept;

    void buffer(glm::vec2 position, uint32_t spriteIndex) noexcept;

    void clear() noexcept;

    void render(const Camera* camera) noexcept;

    int getSpriteCount(void) const noexcept;

    int getSpriteID(const std::string& name) const noexcept;

private:

    void createQuadIndices() noexcept;
    void createQuadVertices(const Sprite* sprite, glm::vec2 origin) noexcept;

private:

    std::map<std::string, int> mSpriteNamesToIndex;

    GLFWwindow* mWindow;
    ShaderProgram mShader;

    // TODO: make sure this is dynamic, in case we go over this limit
    int mMaxQuads = 10000;

    int count;

    GLuint vertexAttributes;

    GLuint vertexBuffer;
    GLuint indexBuffer;

    Vertex* vertexData;
    GLuint* indexData;

    // TODO fix
    SpriteSheet mSpriteSheet = loadSpriteSheet("resources/sprites/smb1_sprites.png");

    Sprite* sprites;
    int spriteCount;
};

#endif // !RENDERER_H_

