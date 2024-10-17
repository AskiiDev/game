#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H


#include "Actor.h"
#include "Player.h"
#include "CollisionUtils.h"
#include "CollisionProfile.h"
#include "CollisionConstants.h"


/**
 * @brief Checks if the player collides with any actors in the world and calculates the collision normal if a collision occurs.
 *
 * This function iterates over all actors in the world and checks for collisions between the player and actors
 * using the actor's collision profile and bounding box. If a collision is detected, it calculates the
 * collision normal using `isSphereInBoundingBox`.
 *
 * @param playerLocation The current world location of the player.
 * @param worldActors A vector of actors in the world to check for collisions.
 * @param[out] collisionNormal The calculated collision normal if a collision occurs.
 * @return True if the player collides with any actor, false otherwise.
 */
bool doesPlayerCollideWithActors(const glm::vec3& playerLocation, std::vector<Actor>& worldActors, glm::vec3& collisionNormal)
{
    for (Actor& actor : worldActors)
    {
        if (!inDistanceForCollisionCheck(playerLocation, actor) ||
            !collisionProfileEnabled(actor, CW_PLAYER)) {
            continue;
        }
        
        BoundingBox box = actor.getBoundingBox();

        if (isCapsuleInBoundingBox(playerLocation, glm::vec3(0, 1, 0), box.min, box.max, collisionNormal, PLAYER_COLLISION_HALF_HEIGHT, PLAYER_COLLISION_RADIUS))
        {
            return true;
        }
    }
    
    return false;
}

/**
 * @brief Moves the player while handling collisions with actors in the world.
 *
 * This function updates the player's position based on its velocity and handles collisions
 * with actors in the world. If a collision occurs, it slides the player along the surface of
 * the colliding object and slightly nudges the player to avoid getting stuck.
 *
 * @param player A pointer to the player object.
 * @param worldActors A vector of actors in the world to check for collisions.
 * @param deltaTime The time delta used to update the player's velocity and movement.
 */
void movePlayerWithCollision(Player* player, std::vector<Actor>& worldActors, const float deltaTime)
{
    glm::vec3 playerVelocity = player->getPlayerVelocity();
    glm::vec3 nextLocation = player->predictNextPlayerLocation(playerVelocity, deltaTime);
    
    glm::vec3 collisionNormal;
    if (!doesPlayerCollideWithActors(nextLocation, worldActors, collisionNormal))
    {
        player->movePlayer(nextLocation);
    }
    else
    {
        glm::vec3 slideVelocity = playerVelocity - glm::dot(playerVelocity, collisionNormal) * collisionNormal;
        
        
        nextLocation = player->predictNextPlayerLocation(slideVelocity, deltaTime);
        if (!doesPlayerCollideWithActors(nextLocation, worldActors, collisionNormal))
        {
            player->movePlayer(nextLocation);
        }
        else
        {
            // If the player still collides, slightly adjust the movement to nudge them away from the edge
            glm::vec3 smallNudge = PLAYER_PUSH_OUT_OF_OBJECT_FORCE * collisionNormal;
            glm::vec3 nudgeVelocity = slideVelocity + smallNudge;

            player->movePlayer(player->predictNextPlayerLocation(nudgeVelocity, deltaTime));
        }
    }
}

#endif
