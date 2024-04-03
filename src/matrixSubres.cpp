#include "MatVK/matrixSubres.hpp"
#include "MatVK/buffer.hpp"
#include <iostream>

namespace matvk
{

    MatrixSubres::MatrixSubres(ElemType elemType, Size2D extents) :
        _elemType(elemType), _extents(extents)
    {
        createImage();
        createImageView();
        setImageLayout();
    }

    void MatrixSubres::createImage()
    {
        uint32_t queueFamilyIndex = VKB::queueFamilyIndex();
        vk::ImageCreateInfo imageCI; imageCI
            .setArrayLayers(1)
            .setExtent(vk::Extent3D(_extents.x, _extents.y, 1))
            .setFormat(formatOfType(_elemType))
            .setImageType(vk::ImageType::e2D)
            .setInitialLayout(vk::ImageLayout::ePreinitialized)
            .setMipLevels(1)
            .setQueueFamilyIndexCount(1)
            .setQueueFamilyIndices(queueFamilyIndex)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setSharingMode(vk::SharingMode::eExclusive)
            .setTiling(vk::ImageTiling::eOptimal)
            .setUsage(vk::ImageUsageFlagBits::eStorage
                | vk::ImageUsageFlagBits::eTransferDst
                | vk::ImageUsageFlagBits::eTransferSrc);
            
        _image = VKB::device().createImage(imageCI);
    
    	auto memRequ = VKB::device().getImageMemoryRequirements(_image);
        vk::MemoryAllocateInfo memoryAI; memoryAI
            .setAllocationSize(memRequ.size)
            .setMemoryTypeIndex(VKB::findMemoryTypeIndex(memRequ.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal));

        _memory = VKB::device().allocateMemory(memoryAI);

        VKB::device().bindImageMemory(_image, _memory, 0);
    }

    void MatrixSubres::createImageView()
    {
        vk::ImageSubresourceRange subresourceRange
            = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

        vk::ImageViewCreateInfo viewCI; viewCI
            .setComponents(vk::ComponentMapping())
            .setFormat(formatOfType(_elemType))
            .setImage(_image)
            .setSubresourceRange(subresourceRange)
            .setViewType(vk::ImageViewType::e2D);

        _imageView = VKB::device().createImageView(viewCI);
    }

    void MatrixSubres::setImageLayout()
    {
        auto cmd = VKB::startOneTimeCommandBuffer();

        vk::ImageSubresourceRange subresourceRange
            = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

        vk::ImageMemoryBarrier imageBarrier; imageBarrier
            .setImage(_image)
            .setSubresourceRange(subresourceRange)
            .setOldLayout(vk::ImageLayout::ePreinitialized)
            .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
            .setSrcAccessMask(vk::AccessFlagBits::eNone)
            .setNewLayout(vk::ImageLayout::eGeneral)
            .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead 
                | vk::AccessFlagBits::eShaderWrite
                | vk::AccessFlagBits::eTransferRead 
                | vk::AccessFlagBits::eTransferWrite);

        cmd.pipelineBarrier(vk::PipelineStageFlagBits::eAllCommands,
            vk::PipelineStageFlagBits::eAllCommands,
            vk::DependencyFlagBits(0),
            {}, {}, imageBarrier);

        VKB::endOneTimeCommandBuffer(cmd);
    }


    void MatrixSubres::write(Size2D extents, Size2D offset, void* src)
    {
        StagingBuffer buffer(extents.x * extents.y * sizeofType(_elemType));
        memcpy(buffer, src, buffer.size());

        vk::ImageSubresourceLayers subresource(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
        vk::BufferImageCopy region; region
            .setImageExtent(vk::Extent3D(extents.x, extents.y, 1))
            .setImageOffset(vk::Offset3D(offset.x, offset.y, 0 ))
            .setImageSubresource(subresource);

        auto cmd = VKB::startOneTimeCommandBuffer();
        cmd.copyBufferToImage(buffer.buffer(), _image, vk::ImageLayout::eGeneral, region);
        VKB::endOneTimeCommandBuffer(cmd);
    }

    void MatrixSubres::read(Size2D extents, Size2D offset, void* dst)
    {
        StagingBuffer buffer(extents.x * extents.y * sizeofType(_elemType));

        vk::ImageSubresourceLayers subresource(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
        vk::BufferImageCopy region; region
            .setImageExtent(vk::Extent3D(extents.x, extents.y, 1))
            .setImageOffset(vk::Offset3D(offset.x, offset.y, 0 ))
            .setImageSubresource(subresource);

        auto cmd = VKB::startOneTimeCommandBuffer();
        cmd.copyImageToBuffer(_image, vk::ImageLayout::eGeneral, buffer.buffer(), region);
        VKB::endOneTimeCommandBuffer(cmd);

        memcpy(dst, buffer, buffer.size());
    }

    ElemType MatrixSubres::type()
    {
        return _elemType;
    }

    Size2D MatrixSubres::extents()
    {
        return Size2D(_extents);
    }

    vk::DescriptorImageInfo MatrixSubres::getDescriptorInfo()
    {
        vk::DescriptorImageInfo info; info
            .setImageLayout(vk::ImageLayout::eGeneral)
            .setImageView(_imageView);

        return info;
    }

    vk::WriteDescriptorSet MatrixSubres::getDescriptorWrite(vk::DescriptorSet set, 
        uint32_t binding, vk::DescriptorImageInfo& info)
    {
        vk::WriteDescriptorSet write; write
            .setDstSet(set)
            .setDstBinding(binding)
            .setDescriptorCount(1)
            .setDescriptorType(vk::DescriptorType::eStorageImage)
            .setDstArrayElement(0)
            .setImageInfo(info);

        return write;
    }

};
