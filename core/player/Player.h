#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"
#include <glm/glm.hpp>
#include <array>

enum Direction
{
    MV_NONE = 0,
    MV_FORWARD = (1 << 0),
    MV_BACKWARD = (1 << 1),
    MV_LEFT = (1 << 2),
    MV_RIGHT = (1 << 3)
};

class Player : public Actor {
private:
    uint8_t movementDirection = 0;
    float playerSpeed = 1.4f;
    
public:
    Player(const Object& o, const Transform& t);
    
    void movePlayerWithInput();
    
    void setMovementDirection(const Direction direction);
    void addMovementDirection(const Direction direction);
    void removeMovementDirection(const Direction direction);
};

#endif
