#ifndef PLAYER_H
#define PLAYER_H

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


struct Camera
{
    glm::vec3 worldLocation;
    glm::vec3 forwardVector;
    glm::vec3 upVector;
    glm::vec3 rightVector;
    glm::vec3 worldUpVector;
    
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    
    std::array<std::array<float, 4>, 6> planes;
    
    double yaw;
    double pitch;
    
    const glm::mat4 getViewProjMatrix()
    {
        return projMatrix * viewMatrix;
    }
};


class Player {
public:
    uint8_t mvDirection = 0;
    
private:
    glm::vec3 playerVelocity = glm::vec3(0, 0, 0);
    float acceleration = 5.0f;
    float deceleration = 15.0f;
    float playerSpeed = 1.2f;
    
    double cameraPitchMin = -87.;
    double cameraPitchMax = 80.;
    
    Camera camera;
    
public:
    Player();
    void init();
    
    Camera getCamera() { return camera; }
    
    void setProjectionMatrix(const glm::mat4& proj);
    
    void addCameraYaw(const float yaw);
    void addCameraPitch(const float pitch);
    
    void updateCameraVectors();
    
    glm::vec3 getPlayerVelocity();
    glm::vec3 getPlayerLocation();
    
    void movePlayer(const glm::vec3& newLocation);
    glm::vec3 predictNextPlayerLocation(const glm::vec3& deltaLocation, const float deltaTime);
};

#endif
