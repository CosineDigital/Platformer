#ifndef CAMERA_H_
#define CAMERA_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

/** @note Disbale camera movement along the x, y, or z axis */
#define ENABLE_X_MOVE 1
#define ENABLE_Y_MOVE 1
#define ENABLE_Z_MOVE 0

/** @note Disable camera look rotation along the x or y axis*/
#define ENABLE_PITCH  0
#define ENABLE_YAW    0

class Camera
{
public:
    friend class Editor;
    friend class Level;

    Camera(const glm::vec3& position_     = glm::vec3(12.0f, 6.5f, 0.0f),
           const glm::vec3& worldUp_      = glm::vec3(0.0f, 1.0f, 0.0f),
           const float& yaw_              = -90.0f, // rotation about the y axis
           const float& pitch_            = 0.0f, // rotation up and down
           const float& lookSensitivity_  = 0.01f);

public:
    glm::mat4 getView(void) const noexcept;

    glm::mat4 getProjection(void) const noexcept;

    glm::mat4 getInverseProjection(void) const noexcept;

    glm::vec2 getPosition2D(void) const noexcept;

public:

    const glm::vec3& getFrontVector(void) const noexcept;

    const glm::vec3& getRightVector(void) const noexcept;

public:
    void update(GLFWwindow* win, float ts) noexcept;

    void updatePos(GLFWwindow* win, float ts) noexcept;

    void updateView(GLFWwindow* win, float ts) noexcept;

private:
    void updateCameraVectors(void) noexcept;

private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;

    float mYaw;
    float mPitch;
    float mLookSensitivity;
    float mZoom;
};

#endif // CAMERA_H_
