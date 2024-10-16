#ifndef ACTOR_H
#define ACTOR_H

#include "VulkanUtils.h"


class Actor {
private:
    BoundingBox cachedBoundingBox;
    bool hasUpdatedSinceLastDraw = true;
    
protected:
    Object obj;
    Transform worldTransform;
    
    float deltaTime;
    
    bool isCulled = true;
    bool isActive = true;
    
    bool activityOverride = false;
    
public:
    
    Actor(Object o, Transform t);
    void update(float deltaTime);
    

    // Getters
    glm::vec3 getWorldLocation();
    glm::vec3 getWorldRotation();
    glm::vec3 getWorldScale();
    
    glm::mat4 getModelMatrix();
    
    glm::vec3 getForwardVector();
    glm::vec3 getRightVector();
    glm::vec3 getUpVector();
    
    Object getObject();
    
    BoundingBox getBoundingBox();
    BoundingBox calculateBoundingBox();
    
    std::vector<glm::vec3> getBoundingBoxCorners();
    
    bool getCulled() { return isCulled; }
    
    
    // Setters
    void setActorLocation(glm::vec3 location);
    void setActorRotation(glm::vec3 rotation);
    void setActorScale(glm::vec3 scale);
    
    void addActorLocation(glm::vec3 addLocation);
    void addActorRotation(glm::vec3 addRotation);
    void addActorScale(glm::vec3 addScale);
    
    void setCulled(bool occlude);

private:
    
    
};

#endif
