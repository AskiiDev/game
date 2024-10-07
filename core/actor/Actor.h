#ifndef ACTOR_H
#define ACTOR_H

#include "VulkanUtils.h"


class Actor {
protected:
    Object obj;
    Transform worldTransform;
    
public:
    Actor(Object o, Transform t);
    
    glm::vec3 getWorldLocation() { return worldTransform.worldLocation; }
    glm::vec3 getWorldRotation() { return worldTransform.worldRotation; }
    glm::vec3 getWorldScale() { return worldTransform.worldScale; }
    
    glm::mat4 getModelMatrix()
    {
        glm::mat4 model = glm::mat4(1.f);
        model = glm::scale(model, getWorldScale());
        
        model = glm::rotate(model, glm::radians(getWorldRotation().x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(getWorldRotation().y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(getWorldRotation().z), glm::vec3(0, 0, 1));
        
        return glm::translate(model, getWorldLocation());
    }
    
    
    glm::vec3 getForwardVector()
    {
        glm::vec3 direction;
        
        direction.x = cos(worldTransform.worldRotation.y) * cos(worldTransform.worldRotation.x);
        direction.y = sin(worldTransform.worldRotation.x);
        direction.z = sin(worldTransform.worldRotation.y) * cos(worldTransform.worldRotation.x);
        
        return glm::normalize(direction);
    }
    
    glm::vec3 getRightVector()
    {
        glm::vec3 forward = getForwardVector();
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        
        return glm::normalize(glm::cross(forward, up));
    }
    
    glm::vec3 getUpVector() {
        glm::vec3 forward = getForwardVector();
        glm::vec3 right = getRightVector();
        
        return glm::normalize(glm::cross(right, forward));
    }

private:
    
    
};

#endif
