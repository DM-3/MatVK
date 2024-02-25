#pragma once

#include <memory>
#include <mutex>

#include "vulkan/vulkan.hpp"


namespace matvk
{

    class VKB
    {
    public:
        VKB(VKB const&) = delete;
        VKB& operator=(VKB const&) = delete;

        static VKB& getVKB();

        vk::Instance instance;
        vk::PhysicalDevice physicalDevice;
        size_t queueFamilyIndex;
        vk::Device device;
        vk::Queue queue;
        vk::CommandPool commandPool;

    private:
        inline static std::unique_ptr<VKB> _vkb{nullptr};
        explicit VKB();

        void createInstance();
        void getPhysicalDevice();
        void selectQueueFamilyIndex();
        void createDevice();
        void getQueue();
        void createCommandPool();
    };

};