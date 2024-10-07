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
    camera.worldLocation = glm::vec3(0.f, 0.3f, 0.f);
    camera.forwardVector = glm::vec3(0.f, 0.f, -1.f);
    camera.worldUpVector = glm::vec3(0.f, 1.f, 0.f);

    camera.yaw = -220.f;
    camera.pitch = 0.f;

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
    glm::vec3 mask = glm::vec3(1, 1, 1);
    
    glm::vec3 forwardDelta = camera.forwardVector * mask;
    glm::vec3 rightDelta = camera.rightVector * mask;
    
    glm::vec3 desiredMovement = glm::vec3(0.0f);
    
    
    if (mvDirection & MV_FORWARD)
    {
        desiredMovement += forwardDelta;
    }
        
    if (mvDirection & MV_BACKWARD)
    {
        desiredMovement -= forwardDelta;
    }
        
    if (mvDirection & MV_LEFT)
    {
        desiredMovement -= rightDelta;
    }
        
    if (mvDirection & MV_RIGHT)
    {
        desiredMovement += rightDelta;
    }

    if (glm::length(desiredMovement) > 0.0f)
    {
        desiredMovement = glm::normalize(desiredMovement);
    }

    glm::vec3 targetVelocity = desiredMovement * playerSpeed;

    if (glm::length(targetVelocity) > 0.0f)
    {
        playerVelocity = glm::mix(playerVelocity, targetVelocity, acceleration * deltaTime);
    }
    else
    {
        playerVelocity = glm::mix(playerVelocity, glm::vec3(0.0f), deceleration * deltaTime);
    }

    camera.worldLocation += playerVelocity * deltaTime;
}

