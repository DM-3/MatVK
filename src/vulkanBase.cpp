#include "MatVK/vulkanBase.hpp"

namespace matvk
{

    std::mutex singleton_mutex;

    VKB& VKB::getVKB()
    {
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if (!_vkb)
            _vkb = std::unique_ptr<VKB>(new VKB());

        return *_vkb;
    }

    VKB::VKB()
    {
        createInstance();
        getPhysicalDevice();
        selectQueueFamilyIndex();
        createDevice();
        getQueue();
        createCommandPool();
    }

    void VKB::createInstance()
    {}

    void VKB::getPhysicalDevice()
    {}

    void VKB::selectQueueFamilyIndex()
    {}

    void VKB::createDevice()
    {}

    void VKB::getQueue()
    {}

    void VKB::createCommandPool()
    {}

};
