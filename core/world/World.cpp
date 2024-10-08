#include "World.h"


World::World()
{
}


void World::load()
{
    Transform t = { glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 45.f, 0.f), glm::vec3(1.f) };
    Object o;
    Actor a(o, t);
    
    worldActors.push_back(a);
}
