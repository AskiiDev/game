#include "Actor.h"
#include <GLFW/glfw3.h>


Actor::Actor(const Object o, const Transform t)
{
    obj = o;
    worldTransform = t;
}


Actor::Actor(const Object o, const Transform t, const CollisionProfile cp) : Actor(o, t)
{
    collisionProfile = cp;
}


void Actor::update(const double dt)
{
    deltaTime = dt;
}


glm::vec3 Actor::getWorldLocation() const { return worldTransform.worldLocation; }
glm::vec3 Actor::getWorldRotation() const { return worldTransform.worldRotation; }
glm::vec3 Actor::getWorldScale() const { return worldTransform.worldScale; }

glm::vec3 Actor::getActorVelocity() const { return actorVelocity; }

glm::mat4 Actor::getModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.f);
    
    model = glm::translate(model, getWorldLocation());
    
    model = glm::rotate(model, glm::radians(getWorldRotation().z), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(getWorldRotation().y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(getWorldRotation().x), glm::vec3(1, 0, 0));
    
    model = glm::scale(model, getWorldScale());
    
    return model;
}


glm::vec3 Actor::getForwardVector() const
{
    glm::vec3 direction;
    
    direction.x = cos(worldTransform.worldRotation.y) * cos(worldTransform.worldRotation.x);
    direction.y = sin(worldTransform.worldRotation.x);
    direction.z = sin(worldTransform.worldRotation.y) * cos(worldTransform.worldRotation.x);
    
    return glm::normalize(direction);
}

glm::vec3 Actor::getRightVector() const
{
    glm::vec3 forward = getForwardVector();
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    return glm::normalize(glm::cross(forward, up));
}

glm::vec3 Actor::getUpVector() const
{
    glm::vec3 forward = getForwardVector();
    glm::vec3 right = getRightVector();
    
    return glm::normalize(glm::cross(right, forward));
}


Object Actor::getObject() const
{
    return obj;
}


void Actor::setActorLocation(const glm::vec3& location)
{
    if (!hasUpdatedSinceLastDraw)
        hasUpdatedSinceLastDraw = true;
    
    worldTransform.worldLocation = location;
}


void Actor::setActorRotation(const glm::vec3& rotation)
{
    if (!hasUpdatedSinceLastDraw)
        hasUpdatedSinceLastDraw = true;
    
    worldTransform.worldRotation = rotation;
}


void Actor::setActorScale(const glm::vec3& scale)
{
    if (!hasUpdatedSinceLastDraw)
        hasUpdatedSinceLastDraw = true;
    
    worldTransform.worldScale = scale;
}

void Actor::addActorLocation(const glm::vec3& addLocation)
{
    setActorLocation(worldTransform.worldLocation + addLocation * deltaTime);
    setActorVelocity(addLocation);
}

void Actor::addActorRotation(const glm::vec3& addRotation)
{
    setActorRotation(worldTransform.worldRotation + addRotation * deltaTime);
}

void Actor::addActorScale(const glm::vec3& addScale)
{
    setActorScale(worldTransform.worldScale + addScale * deltaTime);
}

void Actor::setActorVelocity(const glm::vec3 &velocity)
{
    actorVelocity = velocity;
}

void Actor::setCulled(const bool occlude)
{
    isCulled = occlude;
}

void Actor::setActive(const bool active)
{
    isActive = active;
}

void Actor::setCollisionProfile(const CollisionProfile& cp)
{
    collisionProfile = cp;
}

void Actor::setCollisionSurface(const CollisionSurface& cs)
{
    collisionSurface = cs;
}


BoundingBox Actor::getBoundingBox()
{
    if (hasUpdatedSinceLastDraw)
    {
        cachedBoundingBox = calculateBoundingBox();
        hasUpdatedSinceLastDraw = false;
    }
    
    return cachedBoundingBox;
}


BoundingBox Actor::calculateBoundingBox() const
{
    BoundingBox box = obj.boundingBox;

    // Get the 8 corners of the original bounding box in object space
    glm::vec3 min = box.min;
    glm::vec3 max = box.max;

    std::vector<glm::vec3> corners = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, max.y, max.z)
    };

    // Initialize transformed bounding box
    glm::vec3 transformedMin(FLT_MAX);
    glm::vec3 transformedMax(-FLT_MAX);

    // Apply the actor's world transformation to all corners
    for (const auto& corner : corners)
    {
        glm::vec3 transformedCorner = glm::vec3(getModelMatrix() * glm::vec4(corner, 1.0f));

        // Update the transformed bounding box
        transformedMin = glm::min(transformedMin, transformedCorner);
        transformedMax = glm::max(transformedMax, transformedCorner);
    }

    // Set the transformed bounding box
    BoundingBox transformedBox;
    transformedBox.min = transformedMin;
    transformedBox.max = transformedMax;
    
    return transformedBox;
}


std::vector<glm::vec3> Actor::getBoundingBoxCorners()
{
    BoundingBox box = getBoundingBox();
    
    glm::vec3 min = box.min;
    glm::vec3 max = box.max;

    return {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, max.y, max.z)
    };
}


float Actor::getApproximateBoundingRadius()
{
    BoundingBox box = getBoundingBox();
    
    glm::vec3 min = box.min;
    glm::vec3 max = box.max;
    
    return glm::max(max.x - min.x, glm::max(max.y - min.y, max.z - min.z)) / 2.0f;
}

