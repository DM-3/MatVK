#include "MatVK/vulkanBase.hpp"

namespace matvk
{
    const vk::Instance& VKB::instance()
    {
        if (_vkb) return _vkb->_instance;
        else return VKB::getVKB()._instance;
    }

    const vk::PhysicalDevice& VKB::physicalDevice()
    {
        if (_vkb) return _vkb->_physicalDevice;
        else return VKB::getVKB()._physicalDevice;
    }

    const size_t VKB::queueFamilyIndex()
    {
        if (_vkb) return _vkb->_queueFamilyIndex;
        else return VKB::getVKB()._queueFamilyIndex;
    }

    const vk::Device& VKB::device()
    {
        if (_vkb) return _vkb->_device;
        else return VKB::getVKB()._device;
    }

    const vk::Queue& VKB::queue()
    {
        if (_vkb) return _vkb->_queue;
        else return VKB::getVKB()._queue;
    }

    const vk::CommandPool& VKB::commandPool()
    {
        if (_vkb) return _vkb->_commandPool;
        else return VKB::getVKB()._commandPool;
    }
    
    const uint32_t VKB::subgroupSize()
    {
        if (_vkb) return _vkb->_subgroupSize;
        else return VKB::getVKB()._subgroupSize;
    }


    const uint32_t VKB::findMemoryTypeIndex(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
    {
        auto props = physicalDevice().getMemoryProperties();
       	for (uint32_t i = 0; i < props.memoryTypeCount; i++)
            if ((typeFilter & (1 << i)) && (props.memoryTypes[i].propertyFlags & properties) == properties)
                return i;

    	throw std::runtime_error("Unable to find memory with type: " + std::to_string(typeFilter)
            + " and properties: " + std::to_string(uint32_t(properties)));
        return 0;
    }


    vk::CommandBuffer VKB::startOneTimeCommandBuffer() {
        vk::CommandBufferAllocateInfo commandBufferAI; commandBufferAI
            .setCommandBufferCount(1)
            .setCommandPool(commandPool())
            .setLevel(vk::CommandBufferLevel::ePrimary);

        vk::CommandBufferBeginInfo commandBufferBI; commandBufferBI
            .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        auto commandBuffer = device().allocateCommandBuffers(commandBufferAI)[0];
        commandBuffer.begin(commandBufferBI);

        return commandBuffer;
    }

    void VKB::endOneTimeCommandBuffer(vk::CommandBuffer commandBuffer) {
        commandBuffer.end();

        vk::Fence fence = device().createFence(vk::FenceCreateInfo());
        vk::SubmitInfo submit; submit
            .setCommandBufferCount(1)
            .setCommandBuffers(commandBuffer);

        queue().submit(submit, fence);

        device().waitForFences(fence, vk::True, std::numeric_limits<uint64_t>::max());
        device().freeCommandBuffers(commandPool(), commandBuffer);
    }


    VKB::VKB()
    {
        createInstance();
        getPhysicalDevice();
        selectQueueFamilyIndex();
        createDevice();
        createCommandPool();
    }

    std::mutex singleton_mutex;
    VKB& VKB::getVKB()
    {
        std::lock_guard<std::mutex> lock(singleton_mutex);
        if (!_vkb)
            _vkb = std::unique_ptr<VKB>(new VKB());

        return *_vkb;
    }


    void VKB::createInstance()
    {
        // remove unsupported validation layers
        auto props = vk::enumerateInstanceLayerProperties();
        for (auto it = _validationLayers.begin(); it != _validationLayers.end(); ) {
            if (std::find_if(props.begin(), props.end(), [it](const auto& property)
                { return strcmp(property.layerName, *it) == 0; }) == props.end())
                it = _validationLayers.erase(it);
            else it++;
        }
        
        vk::ApplicationInfo appI; appI
            .setApiVersion(vk::ApiVersion13);

        vk::InstanceCreateInfo instanceCI; instanceCI
            .setPApplicationInfo(&appI)
            .setEnabledLayerCount(_validationLayers.size())
            .setPEnabledLayerNames(_validationLayers);

        _instance = vk::createInstance(instanceCI);
    }

    void VKB::getPhysicalDevice()
    {
        auto devices = _instance.enumeratePhysicalDevices();
        if (devices.empty()) {
            throw std::runtime_error("Couldn't find a physical device.");
            return;
        }
        // TODO: select physical device based on more details  
        _physicalDevice = devices.front();
    }

    void VKB::selectQueueFamilyIndex()
    {
        auto props = _physicalDevice.getQueueFamilyProperties();
	
        // TODO: select queue family based on more details
	    _queueFamilyIndex = std::find_if(props.begin(), props.end(), [](const auto& prop)
		    { return prop.queueFlags & vk::QueueFlagBits::eCompute; }) - props.begin();
    }

    void VKB::createDevice()
    {
        std::vector<float> priorities = { 1.0 };
        vk::DeviceQueueCreateInfo queueCI; queueCI
            .setQueueFamilyIndex(_queueFamilyIndex)
            .setQueueCount(1)
            .setQueuePriorities(priorities);

        vk::DeviceCreateInfo deviceCI; deviceCI
            .setQueueCreateInfoCount(1)
            .setQueueCreateInfos(queueCI);

        _device = _physicalDevice.createDevice(deviceCI);
        _queue = _device.getQueue(_queueFamilyIndex, 0);
    }

    void VKB::createCommandPool()
    {
        vk::CommandPoolCreateInfo commandPoolCI; commandPoolCI
		    .setQueueFamilyIndex(_queueFamilyIndex);

	    _commandPool = _device.createCommandPool(commandPoolCI);
    }

};
