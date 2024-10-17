#ifndef COLLISIONPROFILE_H
#define COLLISIONPROFILE_H


/**
 * @enum CollisionProfile
 * @brief Enum representing different collision profiles for game entities.
 *
 * This enum defines various collision profiles used in the game to categorize
 * entities based on their interaction types. The values can be combined using
 * bitwise operations to create custom collision profiles.
 *
 * @var CollisionProfile::CW_NONE
 * No collision profile assigned.
 *
 * @var CollisionProfile::CW_PLAYER
 * Collision profile for player entities. This profile is used to detect
 * collisions specific to player interactions.
 *
 * @var CollisionProfile::CW_ACTOR
 * Collision profile for non-player actor entities. This profile is used
 * to handle collisions between various actors in the game.
 *
 * @var CollisionProfile::CW_DEFAULT
 * Default collision profile that includes both player and actor profiles.
 * This profile is useful for scenarios where both types of interactions
 * need to be considered simultaneously.
 */
enum CollisionProfile
{
    CW_NONE = 0,
    CW_PLAYER = (1 << 0),
    CW_ACTOR = (1 << 1),
    CW_DEFAULT = (CW_PLAYER | CW_ACTOR)
};

struct CollisionSurface
{
    float friction = 1.f;
};


#endif
