#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);

    glm::vec3 Position;
    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;
    float Pitch;
    float Yaw;
    float senseX = 0.1f;  
    float senseY = 0.1f;
    float speedX = 0.0f; 
    float speedY = 0.0f; 
    float speedZ = 0.0f; 
    float cameraSpeed = 0.1f;

    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float deltaX, float deltaY);
    void UpdateCameraPos();

private:
    void UpdateCameraVectors();
};