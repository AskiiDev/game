#include "Actor.h"
#include "Player.h"


const float CULL_COLLISION_CHECK_PLAYER = 3.f;

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
void calculateBoundingBoxCollisionNormal(const glm::vec3 worldLocation, const glm::vec3 min, const glm::vec3 max, glm::vec3& collisionNormal) {
    
    glm::vec3 deltaMin = worldLocation - min;
    glm::vec3 deltaMax = max - worldLocation;

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
}

/**
 * @brief Checks if a sphere is within a bounding box and calculates the collision normal if a collision occurs.
 *
 * This function checks if a sphere with a given radius intersects or is within a bounding box.
 * If the sphere is inside the bounding box, the function calculates the collision normal using the
 * `calculateCollisionNormal` function.
 *
 * @param worldLocation The world location of the center of the sphere.
 * @param min The minimum corner of the bounding box.
 * @param max The maximum corner of the bounding box.
 * @param[out] collisionNormal The calculated collision normal, which will be set if a collision is detected.
 * @param radius The radius of the sphere (default is 0.12).
 * @return True if the sphere is inside the bounding box, false otherwise.
 */
bool isSphereInBoundingBox(const glm::vec3 worldLocation, const glm::vec3 min, const glm::vec3 max, glm::vec3& collisionNormal, const float radius = 0.12)
{
    if (worldLocation.x >= min.x - radius && worldLocation.x <= max.x + radius &&
        worldLocation.y >= min.y - radius && worldLocation.y <= max.y + radius &&
        worldLocation.z >= min.z - radius && worldLocation.z <= max.z + radius) {
        
        calculateBoundingBoxCollisionNormal(worldLocation, min, max, collisionNormal);
        
        return true;
    }
    
    return false;
}


/**
 * @brief Checks if the player collides with any actors in the world and calculates the collision normal if a collision occurs.
 *
 * This function iterates over all actors in the world and checks for collisions between the player and actors
 * using the actor's collision profile and bounding box. If a collision is detected, it calculates the
 * collision normal using `isSphereInBoundingBox`.
 *
 * @param playerWorldLocation The current world location of the player.
 * @param worldActors A vector of actors in the world to check for collisions.
 * @param[out] collisionNormal The calculated collision normal if a collision occurs.
 * @return True if the player collides with any actor, false otherwise.
 */
bool doesPlayerCollideWithActors(const glm::vec3 playerWorldLocation, std::vector<Actor>& worldActors, glm::vec3& collisionNormal)
{
    for (Actor& actor : worldActors)
    {
        // collision profile collides with player
        if ((actor.getCollisionProfile() bitand CollisionProfile::CW_PLAYER) == 0 && actor.getActive())
        {
            continue;
        }
        
        if (glm::distance(playerWorldLocation, actor.getWorldLocation()) > (actor.getApproximateBoundingRadius() + CULL_COLLISION_CHECK_PLAYER)) {
            continue;
        }
        
        BoundingBox box = actor.getBoundingBox();

        if (isSphereInBoundingBox(playerWorldLocation, box.min, box.max, collisionNormal, 0.12f))
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

            player->movePlayerDelta(nudgeMoveLocation);
        }
    }
}
