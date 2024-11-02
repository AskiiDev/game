#ifndef COLLISIONUTILS_H
#define COLLISIONUTILS_H


#include "Actor.h"
#include <glm/glm.hpp>
#include "CollisionData.h"
#include "CollisionConstants.h"


bool collisionProfileEnabled(
    Actor& actor,
    CollisionProfile profile
)
{
    return actor.getCollisionProfile() & profile;
}


bool inDistanceForCollisionCheck(
    const glm::vec3 playerLocation,
    Actor& actor
)
{
    if (!actor.getActive())
        return false;
    
    return glm::distance(playerLocation, actor.getWorldLocation()) < (actor.getApproximateBoundingRadius() + PLAYER_CULL_COLLISION_DISTANCE);
}

/**
 * @brief Calculates the collision normal of an object based on its world location within a bounding box.
 *
 * This function determines the collision normal by calculating the minimum distances
 * from the object to the bounding box boundaries along each axis (x, y, z) and
 * selecting the axis with the smallest distance.
 *
 * @param worldLocation The current world location of the object.
 * @param min The minimum corner of the bounding box (i.e., the "bottom-left-back" corner).
 * @param max The maximum corner of the bounding box (i.e., the "top-right-front" corner).
 * @param[out] collisionNormal The calculated collision normal, which will be set to the appropriate direction (x, y, or z axis).
 */
void calculateBoundingBoxCollisionNormal(
    const glm::vec3 worldLocation,
    const glm::vec3 min,
    const glm::vec3 max,
    glm::vec3& collisionNormal,
    float& penetrationDepth
)
{
    glm::vec3 deltaMin = worldLocation - min;
    glm::vec3 deltaMax = max - worldLocation;
    
    float nearestX = (deltaMin.x < deltaMax.x) ? deltaMin.x : deltaMax.x;
    float nearestY = (deltaMin.y < deltaMax.y) ? deltaMin.y : deltaMax.y;
    float nearestZ = (deltaMin.z < deltaMax.z) ? deltaMin.z : deltaMax.z;
    
    // Choose the axis with the smallest distance to decide the normal
    if (nearestX < nearestY && nearestX < nearestZ)
    {
        collisionNormal = glm::vec3((deltaMin.x < deltaMax.x) ? -1.0f : 1.0f, 0.0f, 0.0f);
        penetrationDepth = nearestX;
    }
    
    else if (nearestY < nearestX && nearestY < nearestZ)
    {
        collisionNormal = glm::vec3(0.0f, (deltaMin.y < deltaMax.y) ? -1.0f : 1.0f, 0.0f);
        penetrationDepth = nearestY;
    }
    
    else
    {
        collisionNormal = glm::vec3(0.0f, 0.0f, (deltaMin.z < deltaMax.z) ? -1.0f : 1.0f);
        penetrationDepth = nearestZ;
    }
    
    if (penetrationDepth < 0.001)
    {
        penetrationDepth = 0;
    }
}

bool isBoxInBoundingBox(
    const glm::vec3& amin,
    const glm::vec3& amax,
    const glm::vec3& bmin,
    const glm::vec3& bmax,
    glm::vec3& collisionNormalA,
    float& penetrationDepthA,
    glm::vec3& collisionNormalB,
    float& penetrationDepthB
)
{
    if (amax.x < bmin.x || amin.x > bmax.x)
       return false;
    
   if (amax.y < bmin.y || amin.y > bmax.y)
       return false;
    
   if (amax.z < bmin.z || amin.z > bmax.z)
       return false;
    
    glm::vec3 closestPointA;
    closestPointA.x = std::max(amin.x, std::min(bmin.x, amax.x));
    closestPointA.y = std::max(amin.y, std::min(bmin.y, amax.y));
    closestPointA.z = std::max(amin.z, std::min(bmin.z, amax.z));
    
    glm::vec3 closestPointB;
    closestPointB.x = std::max(bmin.x, std::min(amin.x, bmax.x));
    closestPointB.y = std::max(bmin.y, std::min(amin.y, bmax.y));
    closestPointB.z = std::max(bmin.z, std::min(amin.z, bmax.z));
    
    calculateBoundingBoxCollisionNormal(closestPointA, bmin, bmax, collisionNormalA, penetrationDepthA);
    calculateBoundingBoxCollisionNormal(closestPointB, amin, amax, collisionNormalB, penetrationDepthB);
    
    return true;
}

/**
 * @brief Checks if a sphere is within a bounding box and calculates the collision normal if a collision occurs.
 *
 * This function checks if a sphere with a given radius intersects or is within a bounding box.
 * If the sphere is inside the bounding box, the function calculates the collision normal using the
 * `calculateCollisionNormal` function.
 *
 * @param sphereOrigin The world location of the center of the sphere.
 * @param min The minimum corner of the bounding box.
 * @param max The maximum corner of the bounding box.
 * @param[out] collisionNormal The calculated collision normal, which will be set if a collision is detected.
 * @param radius The radius of the sphere (default is 0.12).
 * @return True if the sphere is inside the bounding box, false otherwise.
 */
bool isSphereInBoundingBox(
    const glm::vec3& sphereOrigin,
    const glm::vec3& min,
    const glm::vec3& max,
    glm::vec3& collisionNormal,
    float& penetrationDepth,
    const float radius
)
{
    if (sphereOrigin.x >= min.x - radius && sphereOrigin.x <= max.x + radius &&
        sphereOrigin.y >= min.y - radius && sphereOrigin.y <= max.y + radius &&
        sphereOrigin.z >= min.z - radius && sphereOrigin.z <= max.z + radius)
    {
        glm::vec3 closestPoint = glm::clamp(sphereOrigin, min, max);
        glm::vec3 direction = closestPoint - sphereOrigin;
        
        if (glm::length(direction) > 0.f)
        {
            direction = glm::normalize(direction);
        }
        
        calculateBoundingBoxCollisionNormal((sphereOrigin + direction * radius), min, max, collisionNormal, penetrationDepth);
        
        return true;
    }
    
    return false;
}

bool isCapsuleInBoundingBox(
    const glm::vec3& capsuleOrigin,
    const glm::vec3& capsuleOrientation,
    const glm::vec3& min,
    const glm::vec3& max,
    glm::vec3& collisionNormal,
    float& penetrationDepth,
    const float halfHeight,
    const float radius
)
{
    glm::vec3 closestPoint = glm::clamp(capsuleOrigin, min, max);
    glm::vec3 axisPoint = capsuleOrigin + glm::dot(closestPoint - capsuleOrigin, capsuleOrientation) * capsuleOrientation;
    
    float distance = glm::distance(axisPoint, capsuleOrigin);    
    
    if (distance * distance <= halfHeight * halfHeight)
    {
        if (isSphereInBoundingBox(axisPoint, min, max, collisionNormal, penetrationDepth, radius))
            return true;
    }
    
    return false;
}


#endif
