#include "World.h"


World::World()
{
}


void World::load()
{
    player.init();
    
    Transform t = { glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f) };
    Object o = loadObject("res/models/viking_room.obj", 0);
    Actor a(o, t);
    
    t = { glm::vec3(1.2f, 1.5f, 1.1f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f) };
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
}


Player* World::getPlayer()
{
    return &player;
}


void World::pushActor(Actor newActor)
{
    worldActors.push_back(newActor);
}

