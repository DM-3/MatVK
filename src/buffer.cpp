#include "MatVK/buffer.hpp"


namespace matvk
{

    Buffer::Buffer(size_t size, vk::BufferUsageFlags usage, 
        vk::MemoryPropertyFlags memoryProperties) :
        _size(size)
    {
        uint32_t queueFamilyIndex = VKB::queueFamilyIndex();
        vk::BufferCreateInfo bufferCI; bufferCI
            .setQueueFamilyIndexCount(1)
            .setQueueFamilyIndices(queueFamilyIndex)
            .setSharingMode(vk::SharingMode::eExclusive)
            .setSize(size)
            .setUsage(usage);

        _buffer = VKB::device().createBuffer(bufferCI);

        auto memRequ = VKB::device().getBufferMemoryRequirements(_buffer);
        vk::MemoryAllocateInfo memoryAI; memoryAI
            .setAllocationSize(memRequ.size)
            .setMemoryTypeIndex(VKB::findMemoryTypeIndex(
                memRequ.memoryTypeBits, memoryProperties));

        _memory = VKB::device().allocateMemory(memoryAI);

        VKB::device().bindBufferMemory(_buffer, _memory, 0);
    }

    Buffer::~Buffer()
    {
        VKB::device().freeMemory(_memory);
        VKB::device().destroyBuffer(_buffer);
    }

    const size_t Buffer::size()
    {
        return _size;
    }

    const vk::Buffer& Buffer::buffer()
    {
        return _buffer;
    }

    StagingBuffer::StagingBuffer(size_t size) :
        Buffer(size, 
            vk::BufferUsageFlagBits::eTransferSrc |
            vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent)
    {
        _mapping = VKB::device().mapMemory(_memory, 0, size);
    }

    StagingBuffer::operator void *() const
    {
        return _mapping;
    }

};
