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
    
    for (size_t i = 0; i < worldActors.size(); i++)
    {
        Actor& actorA = worldActors[i];
        
        for (size_t j = 0; j < worldActors.size(); j++)
        {
            Actor& actorB = worldActors[j];
            
            if (&actorA == &actorB)
            {
                continue;
            }
            
            if (doesActorCollideWithActor(playerLocation, actorA, actorB, collisionResult))
            {
                if (!actorA.getPhysicsEnabled())
                {
                    continue;
                }
                
                actorA.onActorCollision(collisionResult);
                
                if (actorB.getPhysicsEnabled())
                {
                    actorA.addActorLocation((collisionResult.penetrationInfo.penetrationDepth / 2.f) *
                                            collisionResult.penetrationInfo.collisionNormal);
                    actorB.addActorLocation((collisionResult.penetrationInfo.penetrationDepth / 2.f) *
                                            -collisionResult.penetrationInfo.collisionNormal);
                }
                else
                {
                    actorA.addActorLocation((collisionResult.penetrationInfo.penetrationDepth) *
                                            collisionResult.penetrationInfo.collisionNormal);
                }
                
                if (collisionResult.penetrationInfo.collisionNormal.y > 0.f)
                {
                    actorA.setGravitationalVelocity(0.f);
                }
            }
        }
    }
}

#endif
