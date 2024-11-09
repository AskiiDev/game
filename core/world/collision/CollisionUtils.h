#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H


#include "Actor.h"
#include <glm/glm.hpp>
#include "CollisionData.h"
#include "CollisionConstants.h"


using namespace std;


bool collisionProfileEnabled(
    const Actor& actor,
    CollisionProfile profile
)
{
    return actor.getCollisionProfile() & profile;
}


bool inDistanceForCollisionCheck(
    const glm::vec3 playerLocation,
    const Actor& actor
)
{
    if (!actor.getActive())
        return false;
    
    return glm::distance(playerLocation, actor.getWorldLocation()) < (actor.getApproximateBoundingRadius() + PLAYER_CULL_COLLISION_DISTANCE);
}


void calculateBoundingBoxCollisionNormal(
    const glm::vec3 worldLocation,
    const glm::vec3 min,
    const glm::vec3 max,
    glm::vec3& collisionNormal
)
{
    glm::vec3 deltaMin = worldLocation - min;
    glm::vec3 deltaMax = max - worldLocation;
    
    float nearestX = (deltaMin.x < deltaMax.x) ? deltaMin.x : deltaMax.x;
    float nearestY = (deltaMin.y < deltaMax.y) ? deltaMin.y : deltaMax.y;
    float nearestZ = (deltaMin.z < deltaMax.z) ? deltaMin.z : deltaMax.z;
    
    if (nearestX < nearestY && nearestX < nearestZ)
    {
        collisionNormal = glm::vec3((deltaMin.x < deltaMax.x) ? -1.0f : 1.0f, 0.0f, 0.0f);
    }
    
    else if (nearestY < nearestX && nearestY < nearestZ)
    {
        collisionNormal = glm::vec3(0.0f, (deltaMin.y < deltaMax.y) ? -1.0f : 1.0f, 0.0f);
    }
    
    else
    {
        collisionNormal = glm::vec3(0.0f, 0.0f, (deltaMin.z < deltaMax.z) ? -1.0f : 1.0f);
    }
}

bool calculatePenetration(
    const float amin,
    const float amax,
    const float bmin,
    const float bmax,
    float& penetrationDepth
)
{
    const float d0 = bmax - amin;
    const float d1 = amax - bmin;
    
    if (d0 < 0.f || d1 < 0.f)
    {
        return false;
    }
    
    const float overlap = min(abs(d0), abs(d1));

    if (overlap < penetrationDepth)
    {
        penetrationDepth = (overlap);
    }
    
    return true;
}

bool isBoxInBoundingBox(
    const glm::vec3& amin,
    const glm::vec3& amax,
    const glm::vec3& bmin,
    const glm::vec3& bmax,
    BasicCollisionResponse& collisionResult
)
{
    float penetrationDepth = FLT_MAX;
    
    if (!calculatePenetration(amin.x, amax.x, bmin.x, bmax.x, penetrationDepth)) return false;
    if (!calculatePenetration(amin.y, amax.y, bmin.y, bmax.y, penetrationDepth)) return false;
    if (!calculatePenetration(amin.z, amax.z, bmin.z, bmax.z, penetrationDepth)) return false;
    
    glm::vec3 closestPoint = (amax + amin) / 2.f;

    calculateBoundingBoxCollisionNormal(closestPoint, bmin, bmax, collisionResult.collisionNormal);
    collisionResult.penetrationDepth = (penetrationDepth) * 1.01;
    
//    std::cout << collisionResult.penetrationDepth << std::endl;
    
    return true;
}


bool isSphereInBoundingBox(
    const glm::vec3& sphereOrigin,
    const glm::vec3& min,
    const glm::vec3& max,
    BasicCollisionResponse& collisionResult,
    const float radius
)
{
    if (sphereOrigin.x >= min.x - radius && sphereOrigin.x <= max.x + radius &&
        sphereOrigin.y >= min.y - radius && sphereOrigin.y <= max.y + radius &&
        sphereOrigin.z >= min.z - radius && sphereOrigin.z <= max.z + radius)
    {
        float penetrationDepth = FLT_MAX;
        calculatePenetration(sphereOrigin.x - radius, sphereOrigin.x + radius, min.x, max.x, penetrationDepth);
        calculatePenetration(sphereOrigin.y - radius, sphereOrigin.y + radius, min.y, max.y, penetrationDepth);
        calculatePenetration(sphereOrigin.z - radius, sphereOrigin.z + radius, min.z, max.z, penetrationDepth);
        
        calculateBoundingBoxCollisionNormal(sphereOrigin, min, max, collisionResult.collisionNormal);
        collisionResult.penetrationDepth = penetrationDepth * 1.01f;
        
        
        return true;
    }
    
    return false;
}

bool isCapsuleInBoundingBox(
    const glm::vec3& capsuleOrigin,
    const glm::vec3& capsuleOrientation,
    const glm::vec3& min,
    const glm::vec3& max,
    BasicCollisionResponse& collisionResult,
    const float halfHeight,
    const float radius
)
{
    glm::vec3 orientationHeight = capsuleOrientation * halfHeight;
    glm::vec3 capsuleStart = capsuleOrigin - orientationHeight;
    glm::vec3 capsuleDirection = 2.f * orientationHeight;
    
    float t = glm::clamp(glm::dot(glm::clamp(capsuleOrigin, min, max) - capsuleStart, capsuleDirection) / glm::dot(capsuleDirection, capsuleDirection), 0.0f, 1.0f);
    
    if (isSphereInBoundingBox(capsuleStart + t * capsuleDirection, min, max, collisionResult, radius))
    {
        return true;
    }
    
    return false;
}


#endif
