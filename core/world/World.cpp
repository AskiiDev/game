#include "World.h"
#include "Frustum.h"
#include "CollisionManager.h"

#define PI 3.14159f


World::World()
{
}


void World::load()
{
    player.init();
    
    Transform t = { glm::vec3(-2.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) };
    Object o = loadObject("res/models/viking_room.obj", 0, 2.2f);
    Actor a(o, t);
    
    t = { glm::vec3(1.2f, 1.5f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f) };
    o = loadObject("res/models/sword/sword.obj", 1, 1.f);
    Actor b(o, t);
    
    
    t = { glm::vec3(1.8f, 1.1f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.2f) };
    Actor c(o, t);
    
    
    pushActor(a);
    pushActor(b);
    pushActor(c);
}


void World::update(float deltaTime)
{
    movePlayerWithCollision(getPlayerAsRef(), worldActors, deltaTime);
    frustumCullActors(player, worldActors);
    
//    int i = 0;
    for (Actor& actor : worldActors)
    {
        if (!actor.getCulled())
        {
            actor.update(deltaTime);
//            i++;
        }
    }
    
//    printf("Actors drawn: %d\n", i);
    
    
//    glm::vec3 fv = getPlayer()->getCamera().forwardVector;
//    glm::vec3 rv = getPlayer()->getCamera().rightVector;
//    
//    glm::vec3 offset = fv * 0.7f + rv * 0.2f;
//    
//    worldActors[1].setActorLocation(getPlayer()->getCamera().worldLocation + offset);
//    
//    glm::quat offsetRotation = glm::quat(glm::vec3(
//        glm::radians(42.f),   // roll
//        glm::radians(-45.f),  // yaw
//        glm::radians(180.f)   // pitch
//    ));
//    
//    glm::quat cameraRotation = glm::quat_cast(glm::inverse(getPlayer()->getCamera().viewMatrix));
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
