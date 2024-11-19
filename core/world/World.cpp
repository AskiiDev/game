#include "World.h"
#include "Frustum.h"
#include "CollisionManager.h"
#include <GLFW/glfw3.h>


World::World()
{
}

World::~World()
{
    for (Actor* actor : worldActors)
    {
        delete actor;
    }
    
    worldActors.clear();
}

void World::load(AudioManager* audioManager)
{
    Transform t;
    Object o;
    
    
    /*----------------------------------------------------*/
    audioManager->loadSource(0, "res/sfx/physics/walk.wav");
    audioManager->loadSource(1, "res/sfx/physics/jump.wav");
    audioManager->loadSource(2, "res/sfx/physics/push_box.wav");
    /*----------------------------------------------------*/
    
    
    o = loadObject("res/models/barrel.obj", "res/models/barrel.png");
    t = { glm::vec3(2.f, 0.f, -5.f), glm::vec3(-0.f, 0.f, 0.f), glm::vec3(4.f) };
    
    worldActors.push_back(new Player(o, t));
    worldObjects.push_back(o);
    
    player = dynamic_cast<Player*>(worldActors.back());
    
    if (player == nullptr)
    {
        throw std::runtime_error("oops");
    }
    
    o = loadObject("res/models/cube.obj", "res/models/crate.jpg");
    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(-0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(0.f, 0.f, -6.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    

    t = { glm::vec3(0.f, 0.f, -7.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(0.f, 9.f, -8.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(-3.f, 0.f, -7.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(-3.f, -1.f, -4.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(5.f, 5.f, 5.f) };
    o = loadObject("res/models/big_floor.obj", "res/textures/floor/grass2.jpg");
    
    createActor(o, t);
    
    /*--------------*/
    
    
    o = loadObject("res/models/arch.obj", "res/textures/floor/cobblestone2.jpg");
    
    createActor(o, t);
    createActor(o, t)->addActorLocation(glm::vec3(0, 0, -8.5));
    /*--------------*/
    
    
    for (Actor* actor : worldActors)
    {
        actor->setAudioManager(audioManager);
    }
}

void World::update(const double deltaTime)
{
    // update actors
    for (Actor* actor : worldActors)
    {
        if (actor->getActive())
        {
            collideWorldActors(player->getWorldLocation(), worldActors);
            actor->update(deltaTime);
        }
    }
    
    collideWorldActors(player->getWorldLocation(), worldActors);
    player->movePlayerWithInput();
//    frustumCullActors(player, worldActors);
}

Player* World::getPlayerAsRef()
{
    return player;
}

Actor* World::createActor(const Object& obj, const Transform& transform)
{
    worldActors.push_back(new Actor(obj, transform));
    
    if (std::find(worldObjects.begin(), worldObjects.end(), obj) == worldObjects.end())
    {
        worldObjects.push_back(obj);
    }
    
    return worldActors.back();
}
