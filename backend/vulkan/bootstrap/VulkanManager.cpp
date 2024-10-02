#include "VulkanManager.h"
#include "iostream"


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif


VulkanManager::VulkanManager()
{
}


void VulkanManager::init(Window* window, Player* p)
{
    createInstance();

    surface.init(instance, window->window);
    deviceManager.init(instance, surface, window->window);
    swapChain.init(surface.surface, window, &deviceManager);
    renderPipeline.init(&deviceManager, &swapChain, p);
}


void VulkanManager::draw()
{
    renderPipeline.drawFrame();
}


void VulkanManager::idle()
{
    vkDeviceWaitIdle(deviceManager.device);
}


void VulkanManager::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }
        
    // application info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "game";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
        
        
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
        
    
    // validation layer handling
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }
    
        
    // platform agnosticism - get extensions through glfw
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
        
    createInfo.enabledLayerCount = 0;
        
        
    // beginning with the 1.3.216 Vulkan SDK, VK_KHR_PORTABILITY_subset extension is mandatory
    std::vector<const char*> requiredExtensions;

    for(uint32_t i = 0; i < glfwExtensionCount; i++)
    {
        requiredExtensions.emplace_back(glfwExtensions[i]);
    }

    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        
    // check if instance creation was successful
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}


void VulkanManager::destroy()
{
    renderPipeline.destroyColorResources();
    renderPipeline.destroyDepthBuffer();
    swapChain.destroy();
    renderPipeline.destroy();
    deviceManager.destroy();
    surface.destroy();
    
    vkDestroyInstance(instance, nullptr);
}
