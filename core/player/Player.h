#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>


enum Direction
{
    MV_NONE = 0,
    MV_FORWARD = 1 << 0,
    MV_BACKWARD = 1 << 1,
    MV_LEFT = 1 << 2,
    MV_RIGHT = 1 << 3
};


struct Camera
{
    glm::vec3 worldLocation;
    glm::vec3 forwardVector;
    glm::vec3 upVector;
    glm::vec3 rightVector;
    glm::vec3 worldUpVector;
    
    double yaw;
    double pitch;
};


class Player {
public:
    Camera camera;
    uint8_t mvDirection = 0;
    
private:
    float playerSpeed = 2.f;
    
public:
    Player();
    void init();
    void updateCameraVectors();
    void updatePlayerMovement(float deltaTime);
    
    bool checkIntersection(const float length);
    
};

#endif
