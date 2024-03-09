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


    void MatrixSubres::write(void* src, ElemType elemType)
    {}

    void MatrixSubres::read(void* dst, ElemType elemType)
    {}

};
