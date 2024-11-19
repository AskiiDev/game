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
    
    if (isBoxInBoundingBox(aBox.min, aBox.max, bBox.min, bBox.max, collisionResultA.penetrationInfo))
    {
        collisionResultA.collisionSurface = actorB.getCollisionSurface();
        collisionResultA.collisionPoint = actorA.getWorldLocation();
        collisionResultA.impactVelocity = actorA.getActualActorVelocity();
        
        if (isBoxInBoundingBox(bBox.min, bBox.max, aBox.min, aBox.max, collisionResultB.penetrationInfo))
        {
            collisionResultB.collisionSurface = actorA.getCollisionSurface();
            collisionResultB.collisionPoint = actorB.getWorldLocation();
            collisionResultB.impactVelocity = actorB.getActualActorVelocity();
        }
        
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
    std::vector<Actor*>& worldActors
)
{
    DetailedCollisionResponse collisionResultA;
    DetailedCollisionResponse collisionResultB;
    
    for (size_t i = 0; i < worldActors.size(); i++)
    {
        Actor& actorA = *worldActors[i];
        
        for (size_t j = 0; j < worldActors.size(); j++)
        {
            Actor& actorB = *worldActors[j];
            
            if (&actorA == &actorB)
            {
                continue;
            }
            
            if (doesActorCollideWithActor(playerLocation, actorA, actorB, collisionResultA, collisionResultB))
            {
                if (!actorA.getPhysicsEnabled())
                {
                    continue;
                }
                
                if (actorB.getPhysicsEnabled())
                {
                    actorA.addActorLocation((collisionResultA.penetrationInfo.penetrationDepth / 2.f) *
                                            collisionResultA.penetrationInfo.collisionNormal);
                    actorB.addActorLocation((collisionResultB.penetrationInfo.penetrationDepth / 2.f) *
                                            collisionResultB.penetrationInfo.collisionNormal);
                }
                else
                {
                    actorA.addActorLocation((collisionResultA.penetrationInfo.penetrationDepth) *
                                            collisionResultA.penetrationInfo.collisionNormal);
                }
                
                actorA.onActorCollision(&actorB, collisionResultA);
                actorB.onActorCollision(&actorA, collisionResultB);

                if (collisionResultA.penetrationInfo.collisionNormal.y > 0.f)
                {
                    if (actorA.getGravitationalVelocity() < 0.f)
                    {
                        actorA.setGravitationalVelocity(0.f);
                    }
                }
            }
        }
    }
}

#endif
