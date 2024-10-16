#ifndef ACTOR_H
#define ACTOR_H

#include "VulkanUtils.h"
#include "Collision.h"


class Actor {
private:
    BoundingBox cachedBoundingBox;
    bool hasUpdatedSinceLastDraw = true;
    
protected:
    Object obj;
    Transform worldTransform;
    
    CollisionProfile collisionProfile = CollisionProfile::CW_DEFAULT;
    
    float deltaTime;
    
    bool isCulled = true;
    bool isActive = true;
    
    bool activityOverride = false;
    
public:
    
    Actor(const Object o, const Transform t);
    Actor(const Object o, const Transform t, const CollisionProfile cp);
    
    void update(float deltaTime);
    

    // Getters
    glm::vec3 getWorldLocation() const;
    glm::vec3 getWorldRotation() const;
    glm::vec3 getWorldScale() const;
    
    glm::mat4 getModelMatrix() const;
    
    glm::vec3 getForwardVector() const;
    glm::vec3 getRightVector() const;
    glm::vec3 getUpVector() const;
    
    Object getObject() const;
    
    BoundingBox getBoundingBox();
    BoundingBox calculateBoundingBox() const;
    
    std::vector<glm::vec3> getBoundingBoxCorners();
    float getApproximateBoundingRadius();
    
    CollisionProfile getCollisionProfile() { return collisionProfile; }
    bool getCulled() { return isCulled; }
    bool getActive() { return isActive; }
    
    
    // Setters
    void setActorLocation(const glm::vec3 location);
    void setActorRotation(const glm::vec3 rotation);
    void setActorScale(const glm::vec3 scale);
    
    void addActorLocation(const glm::vec3 addLocation);
    void addActorRotation(const glm::vec3 addRotation);
    void addActorScale(const glm::vec3 addScale);
    
    void setCulled(const bool occlude);
    void setActive(const bool active);
    
    void setCollisionProfile(const CollisionProfile cp);
    

private:
    
    
};

#endif
