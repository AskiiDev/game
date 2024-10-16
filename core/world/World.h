#ifndef WORLD_H
#define WORLD_H

#include "Actor.h"
#include "Player.h"


class World {
private:
    Player player;
    glm::mat4 lastViewMatrix;
    std::vector<Actor> worldActors;
    
public:
    World();
    
    void load();
    void update(float deltaTime);
    
    std::vector<Actor> getWorldActors() { return worldActors; }
    
    Player* getPlayer();
    
private:
    void pushActor(Actor newActor);
    
};

#endif
