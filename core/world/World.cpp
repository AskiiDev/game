#include "World.h"
#include "Frustum.h"
#include "CollisionManager.h"
#include <GLFW/glfw3.h>


World::World() : player(loadObject("res/models/barrel.obj", "res/models/barrel.png"), { glm::vec3(0.f, 0.f, -5.f), glm::vec3(-0.f, 0.f, 0.f), glm::vec3(4.f) })
{
    worldActors.push_back(player);
    worldObjects.push_back(player.getObject());
}


void World::load()
{
    Transform t;
    Object o;

    o = loadObject("res/models/cube.obj", "res/models/crate.jpg");
    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(-0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    

    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(0.f, 9.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(-3.f, -1.f, -4.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(5.f, 1.f, 5.f) };
    o = loadObject("res/models/floor.obj", "res/textures/floor/floor4.jpg");
    
    createActor(o, t)/*->setCollisionProfile(CW_PLAYER)*/;
//    /*--------------*/
}

void World::update(const double deltaTime)
{
//    player->getPlayerVelocity();
//    DEBUG_LogVec3(worldActors[0].getActorVelocity());
//    worldActors[0].setActorVelocity(glm::vec3(player.getPlayerVelocity().x, worldActors[0].getActorVelocity().y, player.getPlayerVelocity().z));


    // update actors
    for (Actor& actor : worldActors)
    {
        if (actor.getActive())
        {
            collideWorldActors(glm::vec3(0), worldActors);
            actor.update(deltaTime);
        }
    }
    
    collideWorldActors(glm::vec3(0), worldActors);
    
    
//    movePlayerWithCollision(getPlayerAsRef(), worldActors, deltaTime);
//    getPlayerAsRef()->updateCameraVectors();
    
    // actor culling
//    frustumCullActors(player, worldActors);

    
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

Actor* World::createActor(const Object& obj, const Transform& transform)
{
    worldActors.push_back(Actor(obj, transform));
    
    if (std::find(worldObjects.begin(), worldObjects.end(), obj) == worldObjects.end())
    {
        worldObjects.push_back(obj);
    }
    
    return &worldActors.back();
}
