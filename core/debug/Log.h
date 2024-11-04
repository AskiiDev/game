#ifndef LOG_H
#define LOG_H

#include <glm/glm.hpp>


void DEBUG_LogVec3(const glm::vec3& vector)
{
    printf("X: %f, Y: %f, Z: %f\n", vector.x, vector.y, vector.z);
}


#endif
