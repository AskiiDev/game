#include "Player.h"

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define FORWARD_VECTOR glm::vec3(-0.7, 0, -0.7)
#define RIGHT_VECTOR glm::vec3(0.7, 0, -0.7)
//#define FORWARD_VECTOR glm::vec3(0, 0, -1)
//#define RIGHT_VECTOR glm::vec3(1, 0, 0)


Player::Player(const Object& o, const Transform& t) : Actor(o, t)
{
    setPhysicsEnabled(true);
}

void Player::setViewMatrix(const glm::mat4& vm)
{
    viewMatrix = vm;
}

void Player::setProjectionMatrix(const glm::mat4& pm)
{
    projectionMatrix = pm;
}

void Player::setMovementDirection(const Direction direction)
{
    movementDirection = direction;
}

void Player::addMovementDirection(const Direction direction)
{
    movementDirection |= direction;
    audioManager->stopSource(0);
}

void Player::removeMovementDirection(const Direction direction)
{
    movementDirection &= ~direction;
    audioManager->stopSource(2);
}

void Player::movePlayerWithInput()
{
    glm::vec3 desiredMovement = glm::vec3(0.0f);
    
    if (movementDirection & MV_FORWARD)
    {
        desiredMovement += FORWARD_VECTOR;
    }
        
    if (movementDirection & MV_BACKWARD)
    {
        desiredMovement -= FORWARD_VECTOR;
    }
        
    if (movementDirection & MV_LEFT)
    {
        desiredMovement -= RIGHT_VECTOR;
    }
        
    if (movementDirection & MV_RIGHT)
    {
        desiredMovement += RIGHT_VECTOR;
    }

    if (glm::length(desiredMovement) > 0.0f)
    {
        if (glm::length(getActualActorVelocity()) > 0.01f)
        {
            bool collided = false;
            for (const auto& partner : collisionPartners)
            {
                if (partner.first && partner.first->getPhysicsEnabled() && partner.second.penetrationInfo.collisionNormal.y == 0.f)
                {
                    
                    collided = true;
                    audioManager->playSource(2);
                    break;
                }
            }
            if (!collided)
            {
                audioManager->stopSource(2);
            }
            
            if (!getIsInAir())
            {
                audioManager->playSource(0);
            }
        }
        else
        {
            audioManager->stopSource(2);
        }
        
        desiredMovement = glm::normalize(desiredMovement);
    }
    
    setMovementVelocity(desiredMovement * playerSpeed);
}

void Player::jump()
{
    if (!getIsInAir())
    {
        audioManager->playSource(1);
        setGravitationalVelocity(jumpHeight);
    }
}

const glm::vec3 Player::calculateProjectionOffset() const
{
    return cameraOffset * projectionDistance / glm::length(cameraOffset);
}
