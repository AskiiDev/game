#include "Actor.h"
#include <GLFW/glfw3.h>


Actor::Actor(Object o, Transform t)
{
    obj = o;
    worldTransform = t;
}


void Actor::update(float dt)
{
    deltaTime = dt;
}


glm::vec3 Actor::getWorldLocation() { return worldTransform.worldLocation; }
glm::vec3 Actor::getWorldRotation() { return worldTransform.worldRotation; }
glm::vec3 Actor::getWorldScale() { return worldTransform.worldScale; }

glm::mat4 Actor::getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.f);
    
    model = glm::translate(model, getWorldLocation());
    
    model = glm::rotate(model, glm::radians(getWorldRotation().x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(getWorldRotation().z), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(getWorldRotation().y), glm::vec3(0, 1, 0));
    
    model = glm::scale(model, getWorldScale());
    
    return model;
}


glm::vec3 Actor::getForwardVector()
{
    glm::vec3 direction;
    
    direction.x = cos(worldTransform.worldRotation.y) * cos(worldTransform.worldRotation.x);
    direction.y = sin(worldTransform.worldRotation.x);
    direction.z = sin(worldTransform.worldRotation.y) * cos(worldTransform.worldRotation.x);
    
    return glm::normalize(direction);
}

glm::vec3 Actor::getRightVector()
{
    glm::vec3 forward = getForwardVector();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    return glm::normalize(glm::cross(forward, up));
}

glm::vec3 Actor::getUpVector() {
    glm::vec3 forward = getForwardVector();
    glm::vec3 right = getRightVector();
    
    return glm::normalize(glm::cross(right, forward));
}


Object Actor::getObject()
{
    return obj;
}


void Actor::setActorLocation(glm::vec3 location)
{
    worldTransform.worldLocation = location;
}


void Actor::setActorRotation(glm::vec3 rotation)
{
    worldTransform.worldRotation = rotation;
}


void Actor::setActorScale(glm::vec3 scale)
{
    worldTransform.worldScale = scale;
}


void Actor::addActorLocation(glm::vec3 addLocation)
{
    worldTransform.worldLocation += addLocation * deltaTime;
}


void Actor::addActorRotation(glm::vec3 addRotation)
{
    worldTransform.worldRotation += addRotation * deltaTime;
}


void Actor::addActorScale(glm::vec3 addScale)
{
    worldTransform.worldScale += addScale * deltaTime;
}
