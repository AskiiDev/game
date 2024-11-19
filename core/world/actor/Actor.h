#ifndef ACTOR_H
#define ACTOR_H

#include "VulkanUtils.h"
#include "CollisionData.h"
#include "AudioManager.h"


class Actor {
private:
    // Object
    Transform worldTransform;
    Object obj;

    // Movement
    glm::vec3 actorVelocity = glm::vec3(0);
    glm::vec3 movementVelocity = glm::vec3(0);
    glm::vec3 lastWorldLocation = glm::vec3(0);
    glm::vec3 actualActorVelocity = glm::vec3(0);

    // Object collision data
    BoundingBox cachedBoundingBox;
    CollisionProfile collisionProfile = CollisionProfile::CW_DEFAULT;
    CollisionSurface collisionSurface;

    // Physics
    bool physicsEnabled = false;
    float deltaTime = 0.f;
    float gravitationalVelocity = 0;
    float gravitationalAcceleration = -0.3;

    // State flags
    bool isCulled = false;
    bool isActive = true;
    bool activityOverride = false;
    
protected:
    AudioManager* audioManager;
    
    std::unordered_map<Actor*, DetailedCollisionResponse> collisionPartners;
    
public:
    Actor(const Object& o, const Transform& t);
    Actor(const Object& o, const Transform& t, const CollisionProfile& cp);
    virtual ~Actor() = default;
    
    virtual void update(const double deltaTime);
    
    
    // Getters
    const glm::vec3& getWorldLocation() const { return worldTransform.worldLocation; }
    const glm::vec3& getWorldRotation() const { return worldTransform.worldRotation; }
    const glm::vec3& getWorldScale() const { return worldTransform.worldScale; }
    
    const glm::vec3& getMovementVelocity() const { return movementVelocity; }
    const glm::vec3& getActorVelocity() const { return actorVelocity; }
    const glm::vec3& getActualActorVelocity() const { return actualActorVelocity; }
    const float getGravitationalVelocity() const { return gravitationalVelocity; }
    
    const glm::mat4 getModelMatrix() const;
    
    const glm::vec3 getForwardVector() const;
    const glm::vec3 getRightVector() const;
    const glm::vec3 getUpVector() const;
    
    const Object& getObject() const { return obj; }
    
    void cacheBoundingBox();
    const BoundingBox& getBoundingBox() const { return cachedBoundingBox; }
    const BoundingBox calculateBoundingBox() const;
    
    const std::vector<glm::vec3> getBoundingBoxCorners() const;
    const float getApproximateBoundingRadius() const;
    
    const CollisionProfile& getCollisionProfile() const { return collisionProfile; }
    const CollisionSurface& getCollisionSurface() const { return collisionSurface; }
    
    const bool getPhysicsEnabled() const { return physicsEnabled; }
    
    const bool getCulled() const { return isCulled; }
    const bool getActive() const { return isActive; }
    
    const bool getIsInAir() const { return abs(gravitationalVelocity) > 0.01f; }
    
    
    // Setters
    void setActorLocation(const glm::vec3& location);
    void setActorRotation(const glm::vec3& rotation);
    void setActorScale(const glm::vec3& scale);
    
    void addActorLocation(const glm::vec3& addLocation);
    void addActorLocationContinuous(const glm::vec3& addLocation);
    void addActorRotation(const glm::vec3& addRotation);
    void addActorScale(const glm::vec3& addScale);
    
    void setMovementVelocity(const glm::vec3& velocity);
    void setActorVelocity(const glm::vec3& velocity);
    
    void setCulled(const bool occlude);
    void setActive(const bool active);
    
    void setCollisionProfile(const CollisionProfile& cp);
    void setCollisionSurface(const CollisionSurface& cs);
    
    void setPhysicsEnabled(const bool enabled);
    
    void setGravitationalAcceleration(const float acceleration);
    void setGravitationalVelocity(const float velocity);
    
    void setAudioManager(AudioManager* am);
    
    void removeCollisionPartners();
    
    // Event Hooks
    virtual void onActorCollision(Actor* otherActor, const DetailedCollisionResponse& collisionResult);
};

#endif
