#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Log.h"
#include "Actor.h"
#include "Player.h"
#include "CollisionUtils.h"
#include "CollisionData.h"
#include "CollisionConstants.h"


bool doesActorCollideWithActor(
    const glm::vec3& playerLocation,
    const Actor& actorA,
    const Actor& actorB,
    DetailedCollisionResponse& collisionResult
)
{
    if (&actorA == &actorB)
    {
        return false;
    }
    
    if (!actorA.getPhysicsEnabled() && !actorB.getPhysicsEnabled())
    {
        return false;
    }
    
    if (!collisionProfileEnabled(actorA, CW_ACTOR) ||
        !collisionProfileEnabled(actorB, CW_ACTOR))
    {
        return false;
    }
    
    if (!inDistanceForCollisionCheck(playerLocation, actorA) &&
        !inDistanceForCollisionCheck(playerLocation, actorB))
    {
        return false;
    }

    const BoundingBox aBox = actorA.getBoundingBox();
    const BoundingBox bBox = actorB.getBoundingBox();
    
    if (isBoxInBoundingBox(aBox.min, aBox.max, bBox.min, bBox.max, collisionResult.penetrationInfo))
    {
        collisionResult.collisionSurface = actorB.getCollisionSurface();
        collisionResult.collisionPoint = actorA.getWorldLocation();
        collisionResult.impactVelocity = actorA.getActorVelocity();
        
        return true;
    }
    
    return false;
}

/**
 * @brief Checks for collisions between the player and all actors in the world.
 *
 * This function iterates through a list of world actors and checks for collisions
 * between the player and each actor. If a collision is detected, it updates the
 * position of both actors based on their collision results.
 *
 * @param playerLocation The current location of the player.
 * @param worldActors A vector of actors in the world to check for collisions.
 */
void collideWorldActors(
    const glm::vec3& playerLocation,
    std::vector<Actor>& worldActors
)
{
    DetailedCollisionResponse collisionResult;
    
    std::vector<Actor*> collidedActors;
    std::vector<glm::vec3> collidedVelocities;
    
    for (size_t i = 0; i < worldActors.size(); i++)
    {
        Actor& actorB = worldActors[i];
        
        for (size_t j = 0; j < worldActors.size(); j++)
        {
            Actor& actorA = worldActors[j];
            
            if (doesActorCollideWithActor(playerLocation, actorA, actorB, collisionResult))
            {
                if (actorA.getPhysicsEnabled())
                {
                    actorA.addActorLocation((collisionResult.penetrationInfo.penetrationDepth) * collisionResult.penetrationInfo.collisionNormal);
                    collidedActors.push_back(&actorA);
//                    collidedVelocities.push_back(actorA.getActorVelocity() * 0.05f + actorB.getActorVelocity() * 0.95f);
                }
            }
        }
    }
    
    for (size_t i = 0; i < collidedActors.size(); ++i)
    {
        collidedActors[i]->cacheBoundingBox();
//        collidedActors[i]->setActorVelocity(collidedVelocities[i]);
    }
}

/**
 * @brief Checks for collisions between the player and a list of world actors.
 *
 * This function evaluates if the player is colliding with any of the provided
 * world actors based on their bounding boxes. If a collision is detected, it
 * populates the collision result with relevant details about the collision.
 *
 * @param playerLocation The current location of the player.
 * @param worldActors A vector of actors in the world to check for collisions.
 * @param collisionResult Reference to a CollisionResult object to store
 *                       details of the collision if it occurs.
 * @return True if a collision with any actor is detected, false otherwise.
 */
bool doesPlayerCollideWithActors(
    const glm::vec3& playerLocation,
    const std::vector<Actor>& worldActors,
    DetailedCollisionResponse& collisionResult
)
{
    bool collisionOccured = false;
    
    BoundingBox box;
    BasicCollisionResponse penetrationInfo;
    
    glm::vec3 accumulatedNormals(0, 0, 0);
    glm::vec3 accumulatedPenetrationNormals(0, 0, 0);
    
    collisionResult.penetrationInfo.penetrationDepth = 0;
    
    for (const Actor& actor : worldActors)
    {
        if (!collisionProfileEnabled(actor, CW_PLAYER))
        {
            continue;
        }
        
        if (!inDistanceForCollisionCheck(playerLocation, actor))
        {
            continue;
        }
        
        box = actor.getBoundingBox();
        
        if (isCapsuleInBoundingBox(playerLocation, glm::vec3(0, 1, 0), box.min, box.max, penetrationInfo, PLAYER_COLLISION_HALF_HEIGHT, PLAYER_COLLISION_RADIUS))
        {
            collisionResult.collisionPoint = playerLocation;
            collisionResult.impactVelocity = actor.getActorVelocity();
            collisionResult.collisionSurface = actor.getCollisionSurface();
            
            if (penetrationInfo.penetrationDepth > collisionResult.penetrationInfo.penetrationDepth)
            {
                collisionResult.penetrationInfo.penetrationDepth = penetrationInfo.penetrationDepth;
            }
            
            accumulatedNormals += penetrationInfo.collisionNormal;
            accumulatedPenetrationNormals += penetrationInfo.getPenetrationNormal();
            
            collisionOccured = true;
        }
    }
    
    if (collisionOccured)
    {
        if (glm::length(accumulatedNormals) > 0.f && glm::length(accumulatedPenetrationNormals) > 0.f)
        {
            collisionResult.penetrationInfo.collisionNormal = glm::normalize(accumulatedPenetrationNormals);
        }
        
        else
        {
            collisionResult.penetrationInfo.collisionNormal = glm::vec3(0, 1, 0);
        }
    }
    
    
    return collisionOccured;
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
void movePlayerWithCollision(
    Player* player,
    const std::vector<Actor>& worldActors,
    const double deltaTime
)
{
    if (!player) return;
    
    glm::vec3 playerVelocity = player->getPlayerVelocity();
    glm::vec3 nextLocation = player->predictNextPlayerLocation(playerVelocity, deltaTime);
    DetailedCollisionResponse collisionResult;
    
    if (doesPlayerCollideWithActors(nextLocation, worldActors, collisionResult))
    {
        playerVelocity -= (collisionResult.collisionSurface.friction) *
                          (playerVelocity - glm::dot(playerVelocity,
                           collisionResult.penetrationInfo.collisionNormal) *
                           collisionResult.penetrationInfo.collisionNormal);
        
        nextLocation = (collisionResult.penetrationInfo.penetrationDepth) *
                        collisionResult.penetrationInfo.collisionNormal +
                        player->predictNextPlayerLocation(playerVelocity, deltaTime);
    }
    
    player->movePlayer(nextLocation);
}

#endif
