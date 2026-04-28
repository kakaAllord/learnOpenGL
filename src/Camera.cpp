#include "Camera.hpp"

Camera::Camera(glm::vec3 position)
    :m_cameraPosition(position){}
Camera::Camera(const float& xPos, const float& yPos, const float& zPos)
    :m_cameraPosition(glm::vec3(xPos, yPos, zPos)){}

void Camera::Move(const CAMERA_MOVEMENT& movement, const float& deltatime)
{   
    if (movement == CAMERA_MOVEMENT::FORWARD)
        m_cameraPosition -= m_speed * deltatime * m_cameraFront;
    if (movement == CAMERA_MOVEMENT::BACK)
        m_cameraPosition += m_speed * deltatime * m_cameraFront;
    if (movement == CAMERA_MOVEMENT::RIGHT)
        m_cameraPosition -= m_speed * deltatime * glm::normalize(glm::cross(m_cameraFront, m_cameraUp)); 
    if (movement == CAMERA_MOVEMENT::LEFT)
        m_cameraPosition += m_speed * deltatime * glm::normalize(glm::cross(m_cameraFront, m_cameraUp)); 
}

void Camera::Zoom(float& fov, const float& yOffset)
{
    fov -= (float)yOffset;
    if ( fov < 1.0f)
        fov = 1.0f;
    if ( fov >  45.0f)
        fov = 45.0f;
}

void Camera::Rotate(float& xOffset, float& yOffset)
{
  
    xOffset *= m_sensitivity;
    yOffset *= m_sensitivity;
    m_yaw += xOffset;
    m_pitch += yOffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw) * cos(glm::radians(m_pitch)));
    m_cameraFront = glm::normalize(front);
}