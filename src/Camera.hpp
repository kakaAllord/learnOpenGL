#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

enum class CAMERA_MOVEMENT{
    FORWARD,
    BACK,
    RIGHT,
    LEFT
};

class Camera{
private:
    glm::vec3 m_cameraPosition     = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraUp      = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
    float m_sensitivity = 0.5f;
    float m_speed = 7.0f;
    float fieldOfView = 45.f;
public:
    Camera(glm::vec3 position);
    Camera(const float& xPos, const float& yPos, const float& zPos);
    ~Camera() = default;
    glm::mat4 GetViewMatrix() { return glm::lookAt(m_cameraPosition, m_cameraFront, m_cameraUp); }
    void Move(const CAMERA_MOVEMENT& movement, const float& deltatime);
    void Zoom(float& fov, const float& yOffset);
    void Rotate(float& xOffset,float& yOffset);
};