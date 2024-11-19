#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"
#include <glm/glm.hpp>
#include <unordered_map>
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
    uint8_t jumpHeight = 7;
    float playerSpeed = 2;
    
    const glm::vec3 cameraOffset = glm::vec3(1, 2, 1);
    const float projectionDistance = 120.f;
    
    /*-----------------------*/
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    
public:
    Player(const Object& o, const Transform& t);
    
    
    void movePlayerWithInput();
    
    void setMovementDirection(const Direction direction);
    void addMovementDirection(const Direction direction);
    void removeMovementDirection(const Direction direction);
    
    const glm::vec3 calculateProjectionOffset() const;
    const glm::vec3& getCameraOffset() const { return cameraOffset; }
    const glm::mat4& getViewMatrix() const { return viewMatrix; };
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; };
    
    void setViewMatrix(const glm::mat4& vm);
    void setProjectionMatrix(const glm::mat4& pm);
    
    void jump();
};

#endif
