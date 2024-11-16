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

void World::load()
{
    Transform t;
    Object o;
    
    
    o = loadObject("res/models/barrel.obj", "res/models/barrel.png");
    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(-0.f, 0.f, 0.f), glm::vec3(4.f) };
    
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
    
    
    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    

    t = { glm::vec3(0.f, 0.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
    
    
    t = { glm::vec3(0.f, 9.f, -5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(4.f) };

    createActor(o, t)->setPhysicsEnabled(true);
    /*--------------*/
//    
    
    t = { glm::vec3(-3.f, -1.f, -4.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(5.f, 1.f, 5.f) };
    o = loadObject("res/models/floor.obj", "res/textures/floor/floor3.jpeg");
    
    createActor(o, t)/*->setCollisionProfile(CW_PLAYER)*/;
//    /*--------------*/
}

void World::update(const double deltaTime)
{
    player->movePlayerWithInput();

    // update actors
    for (Actor* actor : worldActors)
    {
        if (actor->getActive())
        {
            collideWorldActors(glm::vec3(0), worldActors);
            actor->update(deltaTime);
        }
    }
    
    collideWorldActors(glm::vec3(0), worldActors);
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
