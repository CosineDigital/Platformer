#pragma once

#include <iostream>
#include <cmath>

#include <glad/glad.h>

#include "../editor/imgui/imgui.h"
#include "../editor/imgui/imgui_internal.h"
#include "../editor/imgui/imgui_impl_glfw.h"
#include "../editor/imgui/imgui_impl_opengl3.h"
#include "../editor/editor.h"

#include "../core/level/level.h"
#include "../graphics/renderer.h"
#include "../graphics/line_renderer.h"

/**
* @brief Used internally by OpenGL
*/
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void mouse_callback(GLFWwindow* window, int button, int action, int bits);
static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int bits);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
static void glfw_error_callback(int error, const char* description);

class Editor;

class Application final
{
public:
    explicit Application(int width, int height, const char* appTitle);
    ~Application() noexcept;

    void start(void) noexcept;

    void onMouseEvent(GLFWwindow*, int, int, int) noexcept;
    void onKeyboardEvent(GLFWwindow*, int, int, int, int) noexcept;
    void onScrollEvent(GLFWwindow*, double, double) noexcept;
    void onCursorEvent(GLFWwindow*, double, double) noexcept;
    void onWindowResize(GLFWwindow*, int, int) noexcept;

    int mWidth, mHeight;
    float mCursorX, mCursorY;
    float mCursorLastX, mCursorLastY;
    GLFWwindow* mWindow = nullptr;

    Renderer* mRenderer = nullptr;
    LineRenderer* mLineRenderer = nullptr;
    Level* mLevel = nullptr;

private:

    void draw() noexcept;

    enum ApplicationState : int {
        HOME,
        EDITOR,
        LEVEL
    } mState;

    int error = 0;
    Editor* mEditor = nullptr;
};
