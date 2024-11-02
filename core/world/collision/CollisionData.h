#ifndef COLLISIONPROFILE_H
#define COLLISIONPROFILE_H


/**
 * @enum CollisionProfile
 * @brief Enum representing different collision profiles for game entities.
 *
 * This enum defines various collision profiles used in the game to categorize
 * entities based on their interaction types. These profiles help to manage
 * how different types of game entities interact and collide with each other.
 * The enum values can be combined using bitwise operations to create custom
 * profiles for more complex collision handling.
 */
enum CollisionProfile
{
    CW_NONE = 0,
    CW_PLAYER = (1 << 0),
    CW_ACTOR = (1 << 1),
    CW_DEFAULT = (CW_PLAYER | CW_ACTOR)
};

/**
 * @enum SurfaceType
 * @brief Enum representing different types of surfaces for collision detection.
 *
 * This enum defines the type of surface involved in a collision.
 */
enum SurfaceType
{
    DEFAULT
};

/**
 * @struct CollisionSurface
 * @brief Struct representing surface properties involved in a collision.
 *
 * This structure holds properties of the surface where the collision occurred,
 * such as the friction coefficient and the type of surface. These properties
 * influence the behavior of objects during and after the collision.
 */
struct CollisionSurface
{
    // from 0 to 1
    float friction = 0.5f;
    
    SurfaceType surfaceType = SurfaceType::DEFAULT;
};


struct BasicCollisionResponse
{
    glm::vec3 collisionNormal;
    float penetrationDepth;
};

/**
 * @struct CollisionResult
 * @brief Struct representing the result of a collision between entities.
 *
 * This structure contains information about a collision event, including the
 * point of collision, the normal vector at the collision point, the velocity
 * at impact, and the properties of the surface where the collision occurred.
 */
struct DetailedCollisionResponse
{
    glm::vec3 collisionPoint;
    glm::vec3 impactVelocity;
    CollisionSurface collisionSurface;
    BasicCollisionResponse penetrationInfo;
};






#endif
