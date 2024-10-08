#include "World.h"

#define PI 3.14159f


World::World()
{
}


void World::load()
{
    player.init();
    
    Transform t = { glm::vec3(-1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f) };
    Object o = loadObject("res/models/chateau.obj", 0);
    Actor a(o, t);
    
    t = { glm::vec3(1.2f, 1.5f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f) };
    o = loadObject("res/models/sword/sword.obj", 1);
    Actor b(o, t);
    
    
    t = { glm::vec3(1.8f, 1.1f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.2f) };
    Actor c(o, t);
    
    
    pushActor(a);
    pushActor(b);
    pushActor(c);
}


void World::update(float deltaTime)
{
    
    player.updatePlayerMovement(deltaTime);
    
    for (Actor& actor : worldActors)
    {
        actor.update(deltaTime);
    }
    
    glm::vec3 offset = getPlayer()->getCamera().forwardVector * 0.7f + getPlayer()->getCamera().rightVector * 0.2f;
    
    worldActors[1].setActorLocation(getPlayer()->getCamera().worldLocation + offset);
    
    
    glm::vec3 swordRot(0, 0, 0);
    float yaw = glm::radians(getPlayer()->getCamera().yaw);
    float pitch = glm::radians(getPlayer()->getCamera().pitch);

    // Ensure pitch stays within a reasonable range to avoid gimbal lock
    pitch = glm::clamp(pitch, -glm::half_pi<float>(), glm::half_pi<float>());

    // Create rotation matrix based on yaw and pitch
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, yaw, glm::vec3(0, 0, -1));
    rotationMatrix = glm::rotate(rotationMatrix, pitch, glm::vec3(0, 1, 0));

    // Extract the direction from the rotation matrix
    glm::vec3 angles = degrees(glm::vec3(rotationMatrix[2])); // Assuming Z is forward direction in your system

    swordRot.x = angles.y;
    swordRot.y = glm::degrees(yaw);
    swordRot.z = angles.x;
    
    swordRot += glm::vec3(0, -70, 180);
    
    // Set the actor rotation
    worldActors[1].setActorRotation(swordRot);
}


Player* World::getPlayer()
{
    return &player;
}


void World::pushActor(Actor newActor)
{
    worldActors.push_back(newActor);
}

