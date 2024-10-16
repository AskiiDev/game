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

    camera.yaw = 90.f;
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
    
    camera.viewMatrix = glm::lookAt(camera.worldLocation, camera.worldLocation + camera.forwardVector, camera.upVector);

    camera.planes = {
        std::array<float, 4>{ camera.getViewProjMatrix()[0][3] + camera.getViewProjMatrix()[0][0], camera.getViewProjMatrix()[1][3] + camera.getViewProjMatrix()[1][0], camera.getViewProjMatrix()[2][3] + camera.getViewProjMatrix()[2][0], camera.getViewProjMatrix()[3][3] + camera.getViewProjMatrix()[3][0] }, // Left
        std::array<float, 4>{ camera.getViewProjMatrix()[0][3] - camera.getViewProjMatrix()[0][0], camera.getViewProjMatrix()[1][3] - camera.getViewProjMatrix()[1][0], camera.getViewProjMatrix()[2][3] - camera.getViewProjMatrix()[2][0], camera.getViewProjMatrix()[3][3] - camera.getViewProjMatrix()[3][0] }, // Right
        std::array<float, 4>{ camera.getViewProjMatrix()[0][3] + camera.getViewProjMatrix()[0][1], camera.getViewProjMatrix()[1][3] + camera.getViewProjMatrix()[1][1], camera.getViewProjMatrix()[2][3] + camera.getViewProjMatrix()[2][1], camera.getViewProjMatrix()[3][3] + camera.getViewProjMatrix()[3][1] }, // Top
        std::array<float, 4>{ camera.getViewProjMatrix()[0][3] - camera.getViewProjMatrix()[0][1], camera.getViewProjMatrix()[1][3] - camera.getViewProjMatrix()[1][1], camera.getViewProjMatrix()[2][3] - camera.getViewProjMatrix()[2][1], camera.getViewProjMatrix()[3][3] - camera.getViewProjMatrix()[3][1] }, // Bottom
        std::array<float, 4>{ camera.getViewProjMatrix()[0][3] + camera.getViewProjMatrix()[0][2], camera.getViewProjMatrix()[1][3] + camera.getViewProjMatrix()[1][2], camera.getViewProjMatrix()[2][3] + camera.getViewProjMatrix()[2][2], camera.getViewProjMatrix()[3][3] + camera.getViewProjMatrix()[3][2] }, // Near
        std::array<float, 4>{ camera.getViewProjMatrix()[0][3] - camera.getViewProjMatrix()[0][2], camera.getViewProjMatrix()[1][3] - camera.getViewProjMatrix()[1][2], camera.getViewProjMatrix()[2][3] - camera.getViewProjMatrix()[2][2], camera.getViewProjMatrix()[3][3] - camera.getViewProjMatrix()[3][2] }  // Far
    };
}


void Player::setProjectionMatrix(glm::mat4 proj)
{
    camera.projMatrix = proj;
}


glm::vec3 Player::getPlayerVelocity(float deltaTime)
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

    return playerVelocity * deltaTime;
}


void Player::movePlayerDelta(glm::vec3 newLocation)
{
    camera.worldLocation += newLocation;
}


glm::vec3 Player::getPlayerLocation()
{
    return camera.worldLocation;
}


void Player::addCameraYaw(float yaw)
{\
    camera.yaw += yaw;
}


void Player::addCameraPitch(float pitch)
{
    camera.pitch = std::clamp(camera.pitch - pitch, cameraPitchMin, cameraPitchMax);
}
