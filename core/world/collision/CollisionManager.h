#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H


#include "Actor.h"
#include "Player.h"
#include "CollisionUtils.h"
#include "CollisionData.h"
#include "CollisionConstants.h"


bool doesActorCollideWithActor(
    const glm::vec3& playerLocation,
    Actor& actorA,
    Actor& actorB,
    CollisionResult& collisionResultA,
    CollisionResult& collisionResultB
)
{
    if (&actorA == &actorB)
    {
        return false;
    }
    
    if (!collisionProfileEnabled(actorA, CW_ACTOR) ||
        !collisionProfileEnabled(actorB, CW_ACTOR))
    {
        return false;
    }
    
    if (!inDistanceForCollisionCheck(playerLocation, actorA) && !inDistanceForCollisionCheck(playerLocation, actorB))
    {
        return false;
    }
    
    
    BoundingBox aBox = actorA.getBoundingBox();
    BoundingBox bBox = actorB.getBoundingBox();
    
    if (isBoxInBoundingBox(aBox.min, aBox.max, bBox.min, bBox.max, collisionResultA.collisionNormal))
    {
        calculateBoundingBoxCollisionNormal(actorB.getWorldLocation(), aBox.min, aBox.max, collisionResultB.collisionNormal);
        
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


void collideWorldActors(
    const glm::vec3& playerLocation,
    std::vector<Actor>& worldActors,
    const double deltaTime
)
{
    for (size_t i = 0; i < worldActors.size(); ++i)
    {
        for (size_t j = i + 1; j < worldActors.size(); ++j)
        {
            Actor& actorA = worldActors[i];
            Actor& actorB = worldActors[j];
            CollisionResult collisionResultA;
            CollisionResult collisionResultB;
            
            if (doesActorCollideWithActor(playerLocation, actorA, actorB, collisionResultA, collisionResultB))
            {
                actorA.addActorLocation(glm::length(collisionResultA.impactVelocity) * collisionResultA.collisionNormal);
                actorB.addActorLocation(glm::length(collisionResultB.impactVelocity) * collisionResultB.collisionNormal);
            }
        }
    }
}


bool doesPlayerCollideWithActors(
    const glm::vec3& playerLocation,
    std::vector<Actor>& worldActors,
    CollisionResult& collisionResult
)
{
    for (Actor& actor : worldActors)
    {
        if (!inDistanceForCollisionCheck(playerLocation, actor) ||
            !collisionProfileEnabled(actor, CW_PLAYER)) {
            continue;
        }
        
        BoundingBox box = actor.getBoundingBox();

        if (isCapsuleInBoundingBox(playerLocation, glm::vec3(0, 1, 0), box.min, box.max, collisionResult.collisionNormal, PLAYER_COLLISION_HALF_HEIGHT, PLAYER_COLLISION_RADIUS)) {
            collisionResult.collisionPoint = playerLocation;
            collisionResult.impactVelocity = actor.getActorVelocity();
            collisionResult.collisionSurface = actor.getCollisionSurface();
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
    std::vector<Actor>& worldActors,
    const double deltaTime
)
{
    glm::vec3 playerVelocity = player->getPlayerVelocity();
    glm::vec3 nextLocation = player->predictNextPlayerLocation(playerVelocity, deltaTime);
    
    CollisionResult collisionResult;
    
    if (!doesPlayerCollideWithActors(nextLocation, worldActors, collisionResult))
    {
        player->movePlayer(nextLocation);
    }
    else
    {
        glm::vec3 collisionNormal = collisionResult.collisionNormal;
        CollisionSurface surface = collisionResult.collisionSurface;
        
        glm::vec3 slideVelocity = playerVelocity - glm::dot(playerVelocity, collisionNormal) * collisionNormal;
        
        glm::vec3 frictionForce = -surface.friction * slideVelocity;
        glm::vec3 frictionVelocity = slideVelocity + frictionForce;

        if (glm::length(frictionVelocity) > glm::length(slideVelocity)) {
            frictionVelocity = glm::vec3(0.0f);
        }
        
        slideVelocity = frictionVelocity;
        
        nextLocation = player->predictNextPlayerLocation(slideVelocity, deltaTime);
        if (!doesPlayerCollideWithActors(nextLocation, worldActors, collisionResult))
        {
            player->movePlayer(nextLocation);
        }
        else
        {
            player->movePlayer(player->predictNextPlayerLocation(PLAYER_PUSH_OUT_OF_OBJECT_FORCE * glm::length(collisionResult.impactVelocity) * collisionNormal, deltaTime));
        }
    }
}

#endif
