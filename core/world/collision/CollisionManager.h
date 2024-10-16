#include "Actor.h"
#include "Player.h"


bool doesPlayerCollideWithActors(glm::vec3 playerWorldLocation, std::vector<Actor>& worldActors, glm::vec3& collisionNormal)
{
    for (Actor& actor : worldActors)
    {
        std::vector<glm::vec3> corners = actor.getBoundingBoxCorners();
        
        glm::vec3 min = corners.front();
        glm::vec3 max = corners.back();
        
        const float epsilon = 0.12f;

        if (playerWorldLocation.x >= min.x - epsilon && playerWorldLocation.x <= max.x + epsilon &&
            playerWorldLocation.y >= min.y - epsilon && playerWorldLocation.y <= max.y + epsilon &&
            playerWorldLocation.z >= min.z - epsilon && playerWorldLocation.z <= max.z + epsilon)
        {
            glm::vec3 deltaMin = playerWorldLocation - min;
            glm::vec3 deltaMax = max - playerWorldLocation;

            float nearestX = (deltaMin.x < deltaMax.x) ? deltaMin.x : deltaMax.x;
            float nearestY = (deltaMin.y < deltaMax.y) ? deltaMin.y : deltaMax.y;
            float nearestZ = (deltaMin.z < deltaMax.z) ? deltaMin.z : deltaMax.z;

            // Choose the axis with the smallest distance to decide the normal
            if (nearestX < nearestY && nearestX < nearestZ)
                collisionNormal = glm::vec3((deltaMin.x < deltaMax.x) ? -1.0f : 1.0f, 0.0f, 0.0f);
            else if (nearestY < nearestX && nearestY < nearestZ)
                collisionNormal = glm::vec3(0.0f, (deltaMin.y < deltaMax.y) ? -1.0f : 1.0f, 0.0f);
            else
                collisionNormal = glm::vec3(0.0f, 0.0f, (deltaMin.z < deltaMax.z) ? -1.0f : 1.0f);

            return true;
        }
    }
    
    return false;
}


void movePlayerWithCollision(Player* player, std::vector<Actor>& worldActors, const float deltaTime)
{
    glm::vec3 playerVelocity = player->getPlayerVelocity(deltaTime);
    glm::vec3 playerLocation = player->getPlayerLocation();
    
    glm::vec3 collisionNormal;
    if (!doesPlayerCollideWithActors(playerLocation + playerVelocity, worldActors, collisionNormal))
    {
        player->movePlayerDelta(playerVelocity);
    }
    else
    {
        glm::vec3 slideVelocity = playerVelocity - glm::dot(playerVelocity, collisionNormal) * collisionNormal;
        
        if (!doesPlayerCollideWithActors(playerLocation + slideVelocity, worldActors, collisionNormal))
        {
            player->movePlayerDelta(slideVelocity);
        }
        else
        {
            // If the player still collides, slightly adjust the movement to nudge them away from the edge
            glm::vec3 smallNudge = 0.01f * collisionNormal;  // Small adjustment along collision normal
            glm::vec3 nudgeMoveLocation = slideVelocity + smallNudge;

            if (!doesPlayerCollideWithActors(nudgeMoveLocation, worldActors, collisionNormal))
            {
                player->movePlayerDelta(nudgeMoveLocation);
            }
        }
    }
}
