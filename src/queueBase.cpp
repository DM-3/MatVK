#include "MatVK/queueBase.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>


namespace matvk
{

    QueueBase::QueueBase() 
    {
        _fence = VKB::device().createFence(vk::FenceCreateInfo());
    }

    QueueBase::~QueueBase() 
    {
        VKB::device().destroyFence(_fence);

        if (!_scalarBuffer) return;

        VKB::device().destroyPipelineLayout(_pipelineLayout);     
        VKB::device().freeDescriptorSets(_descriptorPool, _descriptorSet);
        VKB::device().destroyDescriptorPool(_descriptorPool);
        VKB::device().destroyDescriptorSetLayout(_descriptorSetLayout);
        VKB::device().freeCommandBuffers(VKB::commandPool(), _commandBuffer);
        _scalarBuffer.reset();        
    }

    void QueueBase::addAssignment(Assignment assignment) 
    {
        _shaders.push_back(
            Shader(*this, std::static_pointer_cast<MatrixBase>
            (assignment.getDestination()))
        );
        assignment.getSource()->record(_shaders.back());
    }

    void QueueBase::compile() 
    {
        createScalarBuffer();
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

    void QueueBase::createScalarBuffer()
    {}

    void QueueBase::createDescriptorSetLayout()
    {
        std::vector<vk::DescriptorSetLayoutBinding> bindings(_matrices.size() + 1);
        bindings[0]
            .setBinding(0)
            .setDescriptorCount(1)
            .setDescriptorType(vk::DescriptorType::eStorageBuffer)
            .setStageFlags(vk::ShaderStageFlagBits::eCompute);

        for (int i = 1; i < bindings.size(); i++) {
            bindings[i]
                .setBinding(i)
                .setDescriptorCount(1)
                .setDescriptorType(vk::DescriptorType::eStorageImage)
                .setStageFlags(vk::ShaderStageFlagBits::eCompute);
        }
        
        vk::DescriptorSetLayoutCreateInfo layoutCI; layoutCI
            .setBindingCount(bindings.size())
            .setBindings(bindings);

        _descriptorSetLayout = VKB::device().createDescriptorSetLayout(layoutCI);
    }

    void QueueBase::createDescriptorPool()
    {}

    void QueueBase::createDescriptorSet()
    {}

    void QueueBase::createPipelines()
    {
        vk::PipelineLayoutCreateInfo layoutCI; layoutCI
            .setSetLayoutCount(1)
            .setSetLayouts(_descriptorSetLayout);

        _pipelineLayout = VKB::device().createPipelineLayout(layoutCI);

        std::vector<vk::ShaderModule> modules;
        std::vector<vk::ComputePipelineCreateInfo> pipelineCIs;
        
        for (auto& shader : _shaders)
        {
            std::string code = shader.compile();
            vk::ShaderModuleCreateInfo moduleCI; moduleCI
                .setCodeSize(code.size())
                .setPCode((const uint32_t*)code.c_str());

            modules.push_back(VKB::device().createShaderModule(moduleCI));

            vk::PipelineShaderStageCreateInfo stageCI; stageCI
                .setModule(modules.back())
                .setPName("main")
                .setStage(vk::ShaderStageFlagBits::eCompute);

            vk::ComputePipelineCreateInfo pipelineCI; pipelineCI
                .setLayout(_pipelineLayout)
                .setStage(stageCI);

            pipelineCIs.push_back(pipelineCI);
        }

        _pipelines = VKB::device().createComputePipelines(
            vk::PipelineCache(), pipelineCIs).value;
    }

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
