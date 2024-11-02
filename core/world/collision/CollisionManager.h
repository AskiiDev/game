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
    
    if (isBoxInBoundingBox(aBox.min, aBox.max, bBox.min, bBox.max, collisionResultA.collisionNormal, collisionResultA.penetrationDepth, collisionResultB.collisionNormal, collisionResultB.penetrationDepth))
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
                actorA.addActorLocation((glm::length(collisionResultA.impactVelocity) + collisionResultA.penetrationDepth) * collisionResultA.collisionNormal);
                
                actorB.addActorLocation((glm::length(collisionResultB.impactVelocity) + collisionResultB.penetrationDepth) * collisionResultB.collisionNormal);
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
    glm::vec3 accumulatedNormals(0, 0, 0);
    glm::vec3 normal(0, 0, 0);
    
    bool collision = false;
    
    for (Actor& actor : worldActors)
    {
        if (!collisionProfileEnabled(actor, CW_PLAYER))
        {
            continue;
        }
        
        if (!inDistanceForCollisionCheck(playerLocation, actor))
        {
            continue;
        }
        
        BoundingBox box = actor.getBoundingBox();

        if (isCapsuleInBoundingBox(playerLocation, glm::vec3(0, 1, 0), box.min, box.max, normal, collisionResult.penetrationDepth, PLAYER_COLLISION_HALF_HEIGHT, PLAYER_COLLISION_RADIUS))
        {
            collisionResult.collisionPoint = playerLocation;
            collisionResult.impactVelocity = actor.getActorVelocity();
            collisionResult.collisionSurface = actor.getCollisionSurface();
            collisionResult.collisionNormal = normal;
            
            accumulatedNormals += normal;

            collision = true;
        }
    }
    
    if (collision)
    {
        if (glm::length(accumulatedNormals) > 0.f)
        {
            collisionResult.collisionNormal = glm::normalize(accumulatedNormals);
        }
        else
        {
            // TODO: ceiling check
            collisionResult.collisionNormal = glm::vec3(0, 1, 0);
        }
    }
    
    return collision;
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
            collisionNormal = collisionResult.collisionNormal;
            
            float impactVelocity = glm::length(collisionResult.impactVelocity);
            if (impactVelocity == 0)
            {
                impactVelocity = 1;
            }
            
            player->movePlayer(
                    player->predictNextPlayerLocation(//slideVelocity +
                            (impactVelocity + collisionResult.penetrationDepth) *
                            collisionNormal,
                    deltaTime
                )
            );
        }
    }
}

#endif
