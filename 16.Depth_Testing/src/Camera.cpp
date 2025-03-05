#include "Camera.h"
#include "glm/detail/func_geometric.hpp"
#include "glm/detail/type_vec.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) {
    Position = position;
    WorldUp = worldup;
    Forward = glm::normalize(position - target);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup) {
    Position = position;
    WorldUp = worldup;
    Pitch = pitch;
    Yaw = yaw;
    Forward.x = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw));
    Forward.y = glm::sin(glm::radians(Pitch));
    Forward.z = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
    Forward = glm::normalize(Forward);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Right, Forward));
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Forward, Up);
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
    Pitch -= deltaY * senseX;
    Yaw -= deltaX * senseY;
    UpdateCameraVectors();
}

void Camera::UpdateCameraPos() {
    Position += Right * (-speedX) * cameraSpeed;
    Position += WorldUp * speedY * cameraSpeed;
    Position += Forward * speedZ * cameraSpeed;
}

void Camera::UpdateCameraVectors() {
    Forward.x = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw));
    Forward.y = glm::sin(glm::radians(Pitch));
    Forward.z = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
    Forward = glm::normalize(Forward);
    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Right, Forward));
}