#include "MatVK/queueBase.hpp"


namespace matvk
{

    QueueBase::QueueBase() 
    {
        _fence = VKB::device().createFence(vk::FenceCreateInfo());
    }

    QueueBase::~QueueBase() 
    {
        VKB::device().destroyFence(_fence);

        if (!_uniformBuffer) return;

        VKB::device().destroyPipelineLayout(_pipelineLayout);     
        VKB::device().freeDescriptorSets(_descriptorPool, _descriptorSet);
        VKB::device().destroyDescriptorPool(_descriptorPool);
        VKB::device().destroyDescriptorSetLayout(_descriptorSetLayout);
        VKB::device().freeCommandBuffers(VKB::commandPool(), _commandBuffer);
        _uniformBuffer.reset();        
    }

    void QueueBase::addAssignment(Assignment assignment) 
    {
        _shaders.push_back(
            Shader(*this, std::static_pointer_cast<MatrixBase>
            (assignment.getDestination()))
        );
        assignment.getSource()->record(_shaders.back());
        _shaders.back().assemble();
    }

    void QueueBase::compile() 
    {
        createUniformBuffer();
        createDescriptorSetLayout();
        createDescriptorPool();
        createDescriptorSet();
        createPipelines();
        createCommandBuffer();
    }

    void QueueBase::execute() 
    {
        updateUniformBuffer();

        vk::SubmitInfo submit; submit
            .setCommandBufferCount(1)
            .setCommandBuffers(_commandBuffer);

        VKB::queue().submit(submit);
    }

    void QueueBase::waitFinished() 
    {
        vk::Result r = VKB::device().waitForFences(_fence, vk::True, std::numeric_limits<uint64_t>::max());
        if (r != vk::Result::eSuccess)
            throw std::runtime_error("Error when waiting for fence: " + std::to_string(int(r)));
    }

    void QueueBase::createUniformBuffer()
    {}

    void QueueBase::createDescriptorSetLayout()
    {}

    void QueueBase::createDescriptorPool()
    {}

    void QueueBase::createDescriptorSet()
    {}

    void QueueBase::createPipelines()
    {}

    void QueueBase::createCommandBuffer()
    {}

    void QueueBase::updateUniformBuffer()
    {}

    std::vector<std::shared_ptr<MatrixSubres>>& QueueBase::matrices()
    {
        return _matrices;
    }

    std::vector<std::shared_ptr<ScalarSubres>>& QueueBase::scalars()
    {
        return _scalars;
    }

};
