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
    
    t = { glm::vec3(0.f, 0.0f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) };
    o = loadObject("res/models/car.obj", 0);
    Actor a(o, t);
    
    t = { glm::vec3(0.5f, 0.f, -6.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) };
    o = loadObject("res/models/car.obj", 1);
    Actor b(o, t);
    
    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.7f, 1.4f) };
    Actor c(o, t);
    
    a.setPhysicsEnabled(true);
    
    worldActors.push_back(a);
    worldActors.push_back(b);
    worldActors.push_back(c);
}

void World::update(const double deltaTime)
{
    // update actors
    for (Actor& actor : worldActors)
    {
        if (actor.getActive())
        {
            actor.update(deltaTime);
        }
    }
    
    worldActors[0].setActorVelocity(glm::vec3(0, 0, -1));
    worldActors[0].addActorLocation(glm::vec3(0, 0, -1));
//    worldActors[0].setActorVelocity(glm::vec3(0, 0, 1));
//    worldActors[2].addActorLocation(glm::vec3(0, 0, 0.5));
    
    
    collideWorldActors(player.getPlayerLocation(), worldActors, deltaTime);
    
    movePlayerWithCollision(getPlayerAsRef(), worldActors, deltaTime);
    getPlayerAsRef()->updateCameraVectors();
    
    // actor culling
    frustumCullActors(player, worldActors);
    
//    worldActors[0].addActorRotation(glm::vec3(45, 45, 45));
//    worldActors[1].addActorRotation(glm::vec3(-110, -110, -110));
//    worldActors[2].addActorRotation(glm::vec3(-50, -90, -20));

    
//    worldActors[0].setActorScale(glm::vec3(0.5f + sin(glfwGetTime() * 2.f) * sin(glfwGetTime() * 2.f),
//                                           1,
//                                           1));
//    
//    worldActors[1].setActorScale(glm::vec3(1,
//                                           1,
//                                           0.5f + 2.f * cos(glfwGetTime() * 2.f) * cos(glfwGetTime() * 2.f)));
    
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

void World::pushActor(Actor& newActor)
{
    worldActors.push_back(newActor);
}

