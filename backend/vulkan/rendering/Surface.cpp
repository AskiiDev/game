#include "Surface.h"


Surface::Surface()
{
}


void Surface::init(VkInstance i, GLFWwindow* w)
{
    instance = i;
    window = w;
    
    createSurface();
}


void Surface::createSurface()
{
    if (!instance || !window)
    {
        throw std::runtime_error("instance or window not initialized!");
    }
    
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}


void Surface::destroy()
{
    vkDestroySurfaceKHR(instance, surface, nullptr);
}
