#ifndef WORLD_H
#define WORLD_H

#include "Actor.h"
#include "Player.h"
#include "AudioManager.h"


class World {
private:
    Player* player;
    std::vector<Actor*> worldActors;
    std::vector<const Object> worldObjects;
    
public:
    World();
   ~World();
    
    void load(AudioManager* audioManager);
    void update(const double deltaTime);
    
    Player* getPlayerAsRef();
    
    const std::vector<Actor*>& getWorldActors() const { return worldActors; }
    const std::vector<const Object>& getWorldObjects() const { return worldObjects; }
    
private:
    Actor* createActor(const Object& obj, const Transform& transform);
    
};

#endif
