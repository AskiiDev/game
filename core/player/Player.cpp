#include "Player.h"

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Player::Player()
{
}


void Player::init()
{
    camera.worldLocation = glm::vec3(0.f, 0.f, 1.f);
    camera.forwardVector = glm::vec3(0.f, 0.f, -1.f);
    camera.worldUpVector = glm::vec3(0.f, 1.f, 0.f);

    camera.yaw = -90.f;
    camera.pitch = 0.f;
    
    updateCameraVectors();
    updateCameraVectors();
}

void Player::updateCameraVectors()
{
    glm::vec3 fV;
    fV.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    fV.y = sin(glm::radians(camera.pitch));
    fV.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    
    camera.forwardVector = glm::normalize(fV);
    camera.rightVector = glm::normalize(glm::cross(camera.forwardVector, camera.worldUpVector));
    camera.upVector = glm::normalize(glm::cross(camera.rightVector, camera.forwardVector));
}


void Player::updatePlayerMovement(float deltaTime)
{
    float velocity = playerSpeed * deltaTime;
    
    glm::vec3 forwardDelta = camera.forwardVector * velocity;
    glm::vec3 rightDelta = camera.rightVector * velocity;
    glm::vec3 totalMovement = glm::vec3(0.0f);
    
    if (mvDirection & MV_FORWARD)
    {
        totalMovement += forwardDelta;
    }
        
    if (mvDirection & MV_BACKWARD)
    {
        totalMovement -= forwardDelta;
    }
        
    if (mvDirection & MV_LEFT)
    {
        totalMovement -= rightDelta;
    }
        
    if (mvDirection & MV_RIGHT)
    {
        totalMovement += rightDelta;
    }
    
    if (totalMovement != glm::vec3(0.0f))
    {
        totalMovement = glm::normalize(totalMovement) * velocity;
        camera.worldLocation += totalMovement;
    }
}
