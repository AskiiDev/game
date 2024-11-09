#ifndef ACTOR_H
#define ACTOR_H

#include "VulkanUtils.h"
#include "CollisionData.h"


class Actor {
private:
    BoundingBox cachedBoundingBox;
    
    bool hasUpdatedSinceLastDraw = true;
    bool physicsEnabled = false;
    
protected:
    Object obj;
    Transform worldTransform;
    
    glm::vec3 actorVelocity;
    
    CollisionProfile collisionProfile = CollisionProfile::CW_DEFAULT;
    CollisionSurface collisionSurface;
    
    float deltaTime;
    
    bool isCulled = true;
    bool isActive = true;
    
    bool activityOverride = false;
    
public:
    
    Actor(const Object o, const Transform t);
    Actor(const Object o, const Transform t, const CollisionProfile cp);
    
    void update(const double deltaTime);
    

    // Getters
    glm::vec3 getWorldLocation() const;
    glm::vec3 getWorldRotation() const;
    glm::vec3 getWorldScale() const;
    
    glm::vec3 getActorVelocity() const;
    
    glm::mat4 getModelMatrix() const;
    
    glm::vec3 getForwardVector() const;
    glm::vec3 getRightVector() const;
    glm::vec3 getUpVector() const;
    
    Object getObject() const;
    
    
    void cacheBoundingBox();
    BoundingBox getBoundingBox() const;
    BoundingBox calculateBoundingBox() const;
    
    std::vector<glm::vec3> getBoundingBoxCorners() const;
    float getApproximateBoundingRadius() const;
    
    CollisionProfile getCollisionProfile() const { return collisionProfile; }
    CollisionSurface getCollisionSurface() const { return collisionSurface; }
    
    bool getPhysicsEnabled() const { return physicsEnabled; }
    
    bool getCulled() const { return isCulled; }
    bool getActive() const { return isActive; }
    
    
    // Setters
    void setActorLocation(const glm::vec3& location);
    void setActorRotation(const glm::vec3& rotation);
    void setActorScale(const glm::vec3& scale);
    
    void addActorLocation(const glm::vec3& addLocation);
    void addActorLocationContinuous(const glm::vec3& addLocation);
    void addActorRotation(const glm::vec3& addRotation);
    void addActorScale(const glm::vec3& addScale);
    
    void setActorVelocity(const glm::vec3& velocity);
    
    void setCulled(const bool occlude);
    void setActive(const bool active);
    
    void setCollisionProfile(const CollisionProfile& cp);
    void setCollisionSurface(const CollisionSurface& cs);
    
    void setPhysicsEnabled(const bool enabled);
};

#endif
