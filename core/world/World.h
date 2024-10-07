#ifndef WORLD_H
#define WORLD_H

#include "Actor.h"


class World {
private:
    std::vector<Actor> worldActors;
    
public:
    World();
    
    void load();
    std::vector<Actor> getWorldActors() { return worldActors; }
    
};

#endif
