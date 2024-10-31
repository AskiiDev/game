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
    void update(const double deltaTime);
    
    Player* getPlayerAsRef();
    
    const std::vector<Actor>& getWorldActors() { return worldActors; }
    
private:
    void pushActor(Actor& newActor);
    
};

#endif
