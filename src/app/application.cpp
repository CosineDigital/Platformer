#include "application.h"


Application::Application(int width, int height, const char* appTitle) : mWidth(width), mHeight(height), 
mCursorX(0.0), mCursorY(0.0), mCursorLastX(0.0), mCursorLastY(0.0), mState(ApplicationState::HOME)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    mWindow = glfwCreateWindow(width, height, appTitle, 0, 0);

    if (!mWindow)
    {
        std::cout << "ERROR\tCould not create a GLFWWindow context!\n";
        this->error = 1;
    }
    else
    {
        glfwSetWindowUserPointer(mWindow, this);
        glfwMakeContextCurrent(mWindow);
        glfwSetFramebufferSizeCallback(mWindow, ::framebuffer_size_callback);
        glfwSetMouseButtonCallback(mWindow, ::mouse_callback);
        glfwSetKeyCallback(mWindow, ::keyboard_callback);
        glfwSetScrollCallback(mWindow, ::scroll_callback);
        glfwSetCursorPosCallback(mWindow, ::cursor_callback);
        glfwSetErrorCallback(::glfw_error_callback);
    }

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
       std::cout << "ERROR\tCould not initialize Glad\n";
       this->error = 1;
    }

    onWindowResize(mWindow, width, height);
}

Application::~Application() noexcept
{
    delete mRenderer;
    delete mLineRenderer;
    delete mEditor;
    delete mLevel;
    glfwTerminate();
    std::cout << "Exited succesfully\n";
}

void Application::start() noexcept
{
    if (this->error) return;

    mLevel = new Level();
    mRenderer = new Renderer();
    mLineRenderer = new LineRenderer();
    mEditor = new Editor(this);
    mEditor->activate();
    mEditor->setLevelForEditing(mLevel);

    while (!glfwWindowShouldClose(mWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        mLevel->draw(mRenderer);
        mRenderer->render(mEditor->mCamera);

        mEditor->draw();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}

void Application::draw() noexcept {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw the ui // TODO: refactor this
    switch (mState) {
    case ApplicationState::HOME:
        ImGui::Begin("Menu");

        if (ImGui::Button("Editor")) {
            mState = ApplicationState::EDITOR;
        }

        ImGui::End();
        break;

    case ApplicationState::EDITOR:
        

        break;
    case ApplicationState::LEVEL:
        break;
    }
        
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::onMouseEvent(GLFWwindow* window, int button, int action, int bits) noexcept {
    mLevel->onMouseEvent(window, button, action, bits);
    mEditor->onMouseEvent(window, button, action, bits);
}

void Application::onKeyboardEvent(GLFWwindow* window, int key, int scancode, int action, int bits) noexcept {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) { glfwSetWindowShouldClose(window, GL_TRUE); }
    mLevel->onKeyboardEvent(window, key, scancode, action, bits);
    mEditor->onKeyboardEvent(window, key, scancode, action, bits);
}

void Application::onScrollEvent(GLFWwindow* window, double xoffset, double yoffset) noexcept {
    mLevel->onScrollEvent(window, xoffset, yoffset);
    mEditor->onScrollEvent(window, xoffset, yoffset);
}

void Application::onCursorEvent(GLFWwindow* window, double xpos, double ypos) noexcept {
    mCursorLastX = mCursorX; mCursorLastY = mCursorY;
    mCursorX = static_cast<float>(xpos); mCursorY = static_cast<float>(ypos);
    mLevel->onCursorEvent(window, xpos, ypos);
    mEditor->onCursorEvent(window, xpos, ypos);
}

void Application::onWindowResize(GLFWwindow* window, int width, int height) noexcept {
    mWidth = width;
    mHeight = height;
}



static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    ((Application*)glfwGetWindowUserPointer(window))->onWindowResize(window, width, height);
    glViewport(0, 0, width, height);
}
static void mouse_callback(GLFWwindow* window, int button, int action, int bits) {
    ((Application*)glfwGetWindowUserPointer(window))->onMouseEvent(window, button, action, bits);
}
static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int bits) {
    ((Application*)glfwGetWindowUserPointer(window))->onKeyboardEvent(window, key, scancode, action, bits);
}
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    ((Application*)glfwGetWindowUserPointer(window))->onScrollEvent(window, xoffset, yoffset);
}
static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    ((Application*)glfwGetWindowUserPointer(window))->onCursorEvent(window, xpos, ypos);
}
static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error - " << error << " - " << description << '\n';
}