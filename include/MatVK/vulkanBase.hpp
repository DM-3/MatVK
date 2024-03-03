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

        static const vk::Instance&          instance();
        static const vk::PhysicalDevice&    physicalDevice();
        static const size_t                 queueFamilyIndex();
        static const vk::Device&            device();
        static const vk::Queue&             queue();
        static const vk::CommandPool&       commandPool();
        static const uint32_t               subgroupSize();

    private:
        inline static std::unique_ptr<VKB> _vkb{nullptr};
        explicit VKB();
        static VKB& getVKB();

        vk::Instance        _instance;
        vk::PhysicalDevice  _physicalDevice;
        size_t              _queueFamilyIndex;
        vk::Device          _device;
        vk::Queue           _queue;
        vk::CommandPool     _commandPool;
        uint32_t            _subgroupSize;

        std::vector<const char*> _validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        void createInstance();
        void getPhysicalDevice();
        void selectQueueFamilyIndex();
        void createDevice();
        void createCommandPool();
    };

};