#include "World.h"
#include "WorldUtils.h"

#define PI 3.14159f


World::World()
{
}


void World::load()
{
    player.init();
    
    Transform t = { glm::vec3(-1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f) };
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
    player.updatePlayerMovement(deltaTime);
    
    cullActors();
    
    int i = 0;
    for (Actor& actor : worldActors)
    {
        if (!actor.getCulled())
            i++;
            
        actor.update(deltaTime);
    }
    
    printf("Actors drawn: %d\n", i);
}


void World::cullActors()
{
    std::array<std::array<float, 4>, 6> planes = getPlayer()->getCamera().planes;

    for (Actor& actor : worldActors)
    {
        BoundingBox boundingBox = actor.getBoundingBox();
        glm::vec3 min = boundingBox.min;
        glm::vec3 max = boundingBox.max;

        std::vector<glm::vec3> corners = {
            glm::vec3(min.x, min.y, min.z),
            glm::vec3(max.x, min.y, min.z),
            glm::vec3(min.x, max.y, min.z),
            glm::vec3(max.x, max.y, min.z),
            glm::vec3(min.x, min.y, max.z),
            glm::vec3(max.x, min.y, max.z),
            glm::vec3(min.x, max.y, max.z),
            glm::vec3(max.x, max.y, max.z)
        };
        
        bool isCulled = true;

        for (int i = 0; i < 6; ++i)
        {
            bool insidePlane = false;

            for (const auto& corner : corners)
            {
                float distance = planes[i][0] * corner.x +
                                 planes[i][1] * corner.y +
                                 planes[i][2] * corner.z +
                                 planes[i][3];

                if (distance >= 0)
                {
                    insidePlane = true;
                    break;
                }
            }

            if (!insidePlane)
            {
                isCulled = true;
                break;
            }
            else
            {
                isCulled = false;
            }
        }

        actor.setCulled(isCulled);
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
