#ifndef ACTOR_H
#define ACTOR_H

#include "VulkanUtils.h"


class Actor {
protected:
    Object obj;
    Transform worldTransform;
    
public:
    Actor(Object o, Transform t);
    
    glm::vec3 getWorldLocation();
    glm::vec3 getWorldRotation();
    glm::vec3 getWorldScale();
    
    glm::mat4 getModelMatrix();
    
    glm::vec3 getForwardVector();
    glm::vec3 getRightVector();
    glm::vec3 getUpVector();

private:
    
    
};

#endif
