#include "World.h"
#include "Frustum.h"
#include "CollisionManager.h"
#include <GLFW/glfw3.h>


World::World()
{
}


void World::load()
{
    player.init();
    
    Transform t;
    Object o;
    
    t = { glm::vec3(0.f, -2.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) };
    o = loadObject("res/models/viking_room.obj", 0);
    Actor a(o, t);
    
    CollisionSurface surface;
    surface.friction = 0.5f;
    
    a.setCollisionSurface(surface);
    
    t = { glm::vec3(1.2f, 1.5f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f) };
    o = loadObject("res/models/sword/sword.obj", 1);
//    o = loadObject("res/models/chateau.obj", 0);
    Actor b(o, t);
    
    t = { glm::vec3(1.8f, 1.1f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.2f) };
    Actor c(o, t);
    
    
    pushActor(a);
    pushActor(b);
    pushActor(c);
}


void World::update(float deltaTime)
{
    // update player
    movePlayerWithCollision(getPlayerAsRef(), worldActors, deltaTime);
    getPlayerAsRef()->updateCameraVectors();
    
    // actor culling
    frustumCullActors(player, worldActors);
    
    // update actors
    for (Actor& actor : worldActors)
    {
        if (!actor.getCulled() && actor.getActive())
        {
            actor.update(deltaTime);
        }
    }
    
    
    
//    glm::vec3 fv = getPlayerAsRef()->getCamera().forwardVector;
//    glm::vec3 rv = getPlayerAsRef()->getCamera().rightVector;
//    
//    glm::vec3 offset = fv * 0.2f + rv * 0.2f;
//    
//    worldActors[1].setActorLocation(getPlayerAsRef()->getCamera().worldLocation + offset);
//    
//    glm::quat offsetRotation = glm::quat(glm::vec3(
//        glm::radians(42.f),   // roll
//        glm::radians(-45.f),  // yaw
//        glm::radians(180.f)   // pitch
//    ));
//    
//    glm::quat cameraRotation = glm::quat_cast(glm::inverse(getPlayerAsRef()->getCamera().viewMatrix));
//    glm::vec3 finalRotation = glm::degrees(glm::eulerAngles(cameraRotation * offsetRotation));
//    
//    worldActors[1].setActorRotation(finalRotation);
}


Player* World::getPlayerAsRef()
{
    return &player;
}


void World::pushActor(Actor newActor)
{
    worldActors.push_back(newActor);
}
