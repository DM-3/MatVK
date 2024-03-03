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
        createCommandPool();
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
