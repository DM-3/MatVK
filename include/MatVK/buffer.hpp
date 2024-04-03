#pragma once

#include "vulkanBase.hpp"


namespace matvk
{

    class Buffer
    {
    public:
        Buffer(size_t size, vk::BufferUsageFlags usage, 
            vk::MemoryPropertyFlags memoryProperties);
        ~Buffer();
        const size_t size();
        const vk::Buffer& buffer();
        vk::DescriptorBufferInfo getDescriptorInfo(uint64_t offset = 0, uint64_t range = vk::WholeSize);
        vk::WriteDescriptorSet getDescriptorWrite(vk::DescriptorSet set, 
            uint32_t binding, vk::DescriptorBufferInfo& info);

    protected:
        size_t _size;
        vk::Buffer _buffer;
        vk::DeviceMemory _memory;
    };

    class StagingBuffer : public Buffer
    {
    public:
        StagingBuffer(size_t size);
        operator void*() const;

    private:
        void* _mapping;
    };

};
