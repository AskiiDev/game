#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Actor.h"
#include "Player.h"
#include "CollisionUtils.h"
#include "CollisionData.h"
#include "CollisionConstants.h"

/**
 * @brief Checks for collision between two actors.
 *
 * This function determines whether two actors are colliding based on their
 * bounding boxes and the player's location. If a collision is detected,
 * it fills the provided collision results with details about the collision.
 *
 * @param playerLocation The current location of the player.
 * @param actorA The first actor to check for collision.
 * @param actorB The second actor to check for collision.
 * @param collisionResultA Reference to a CollisionResult object to store
 *                         results for actorA.
 * @param collisionResultB Reference to a CollisionResult object to store
 *                         results for actorB.
 * @return True if a collision is detected, false otherwise.
 */
bool doesActorCollideWithActor(
    const glm::vec3& playerLocation,
    const Actor& actorA,
    const Actor& actorB,
    DetailedCollisionResponse& collisionResultA,
    DetailedCollisionResponse& collisionResultB
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
    
    if (isBoxInBoundingBox(aBox.min, aBox.max, bBox.min, bBox.max, collisionResultA.penetrationInfo, collisionResultB.penetrationInfo))
    {
        collisionResultA.collisionSurface = actorB.getCollisionSurface();
        collisionResultA.collisionPoint = actorA.getWorldLocation();
        collisionResultA.impactVelocity = actorA.getActorVelocity();
        
        collisionResultB.collisionSurface = actorA.getCollisionSurface();
        collisionResultB.collisionPoint = actorB.getWorldLocation();
        collisionResultB.impactVelocity = actorB.getActorVelocity();
        
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
 * @param deltaTime The time delta used to update the positions of actors.
 */
void collideWorldActors(
    const glm::vec3& playerLocation,
    std::vector<Actor>& worldActors,
    const double deltaTime
)
{
    DetailedCollisionResponse collisionResultA;
    DetailedCollisionResponse collisionResultB;
    
    for (size_t i = 0; i < worldActors.size(); ++i)
    {
        Actor& actorA = worldActors[i];
        
        for (size_t j = i + 1; j < worldActors.size(); ++j)
        {
            Actor& actorB = worldActors[j];
            
            if (doesActorCollideWithActor(playerLocation, actorA, actorB, collisionResultA, collisionResultB))
            {
                if (actorA.getPhysicsEnabled())
                {
                    actorA.addActorLocation((glm::length(collisionResultA.impactVelocity) + collisionResultA.penetrationInfo.penetrationDepth) * collisionResultA.penetrationInfo.collisionNormal);
                }
                
                if (actorB.getPhysicsEnabled())
                {
                    actorB.addActorLocation((glm::length(collisionResultB.impactVelocity) + collisionResultB.penetrationInfo.penetrationDepth) * collisionResultB.penetrationInfo.collisionNormal);
                }
            }
        }
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
    BoundingBox box;
    BasicCollisionResponse penetrationInfo;
    
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
            collisionResult.penetrationInfo = penetrationInfo;
            
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
        glm::vec3 slideVelocity = playerVelocity -
        glm::dot(playerVelocity, collisionResult.penetrationInfo.collisionNormal) *
        collisionResult.penetrationInfo.collisionNormal;
        
        glm::vec3 frictionVelocity = slideVelocity -
                                     collisionResult.collisionSurface.friction *
                                     slideVelocity;

        if (glm::length(frictionVelocity) > glm::length(slideVelocity))
        {
            frictionVelocity = glm::vec3(0.0f);
        }

        nextLocation = player->predictNextPlayerLocation(frictionVelocity, deltaTime);
        
        if (doesPlayerCollideWithActors(nextLocation, worldActors, collisionResult))
        {
            float impactVelocity = glm::length(collisionResult.impactVelocity);
            
            if (impactVelocity == 0)
            {
                impactVelocity = 1;
            }
            
            nextLocation = player->predictNextPlayerLocation(frictionVelocity +
                                  (impactVelocity + collisionResult.penetrationInfo.penetrationDepth) *
                                  collisionResult.penetrationInfo.collisionNormal, deltaTime);
        }
    }
    
    player->movePlayer(nextLocation);
}

#endif
