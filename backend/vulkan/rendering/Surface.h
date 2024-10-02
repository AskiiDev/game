#ifndef SURFACE_H
#define SURFACE_H

#include "VulkanUtils.h"
#include <GLFW/glfw3.h>


class Surface {
public:
    VkSurfaceKHR surface;
    
    VkInstance instance;
    GLFWwindow* window;
    
public:
    Surface();
    
    void init(VkInstance i, GLFWwindow* w);
    void destroy();
    
private:
    void createSurface();
    
};

#endif
