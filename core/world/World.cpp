#include "World.h"


World::World()
{
}


void World::load()
{
    Transform t = { glm::vec3(0.f), glm::vec3(0.f, 45.f, 0.f), glm::vec3(1.f) };
    Actor a(loadObject("res/models/viking_room.obj"), t);
    
    worldActors.push_back(a);
}
