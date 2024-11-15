#include "Player.h"

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Player::Player(const Object& o, const Transform& t) : Actor(o, t)
{
    setPhysicsEnabled(true);
}

void Player::setMovementDirection(const Direction direction)
{
    movementDirection = direction;
}

void Player::addMovementDirection(const Direction direction)
{
    movementDirection |= direction;
}

void Player::removeMovementDirection(const Direction direction)
{
    movementDirection &= ~direction;
}

void Player::movePlayerWithInput()
{
    glm::vec3 forwardDelta = glm::vec3(0, 0, -1);
    glm::vec3 rightDelta = glm::vec3(1, 0, 0);
    
    glm::vec3 desiredMovement = glm::vec3(0.0f);
    
    
    if (movementDirection & MV_FORWARD)
    {
        desiredMovement += forwardDelta;
    }
        
    if (movementDirection & MV_BACKWARD)
    {
        desiredMovement -= forwardDelta;
    }
        
    if (movementDirection & MV_LEFT)
    {
        desiredMovement -= rightDelta;
    }
        
    if (movementDirection & MV_RIGHT)
    {
        desiredMovement += rightDelta;
    }

    if (glm::length(desiredMovement) > 0.0f)
    {
        desiredMovement = glm::normalize(desiredMovement);
    }
    
    glm::vec3 velocity = desiredMovement * playerSpeed;
    velocity.y = getActorVelocity().y;
    
    setActorVelocity(velocity);
}

