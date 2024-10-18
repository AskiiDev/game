#ifndef WORLD_H
#define WORLD_H

#include "Actor.h"
#include "Player.h"


class World {
private:
    Player player;
    std::vector<Actor> worldActors;
    
public:
    World();
    
    void load();
    void update(const float deltaTime);
    
    Player* getPlayerAsRef();
    
    std::vector<Actor> getWorldActors() const { return worldActors; }
    
private:
    void pushActor(Actor& newActor);
    
};

#endif
