#include "Player.h"

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Player::Player(const Object& o, const Transform& t) : Actor(o, t)
{
    setPhysicsEnabled(true);
}

glm::vec3 Player::getPlayerVelocity()
{
    glm::vec3 forwardDelta = glm::vec3(0, 0, -1);
    glm::vec3 rightDelta = glm::vec3(1, 0, 0);
    
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
    
    setActorVelocity(desiredMovement * playerSpeed);
    return playerVelocity;
    
}

void Player::setPlayerVelocity(const glm::vec3& velocity)
{
    playerVelocity = velocity;
}
