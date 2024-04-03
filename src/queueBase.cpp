#include "MatVK/queueBase.hpp"
#include "MatVK/matrixSubres.hpp"
#include "MatVK/scalarSubres.hpp"


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
    {
        size_t size = 0;
        for (auto s : _scalars)
            size += sizeofType(s->type());

        _scalarBuffer = std::make_unique<Buffer>(size, 
            vk::BufferUsageFlagBits::eStorageBuffer |
            vk::BufferUsageFlagBits::eTransferSrc |
            vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal);
    }

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
    {
        std::vector<vk::DescriptorPoolSize> sizes(2);
        sizes[0]
            .setType(vk::DescriptorType::eStorageBuffer)
            .setDescriptorCount(1);
        sizes[1]
            .setType(vk::DescriptorType::eStorageImage)
            .setDescriptorCount(_matrices.size());

        vk::DescriptorPoolCreateInfo poolCI; poolCI
            .setMaxSets(1)
            .setPoolSizes(sizes);

        _descriptorPool = VKB::device().createDescriptorPool(poolCI);
    }

    void QueueBase::createDescriptorSet()
    {
        // allocate descriptor set
        vk::DescriptorSetAllocateInfo setAI; setAI
            .setDescriptorPool(_descriptorPool)
            .setDescriptorSetCount(1)
            .setSetLayouts(_descriptorSetLayout);

        _descriptorSet = VKB::device().allocateDescriptorSets(setAI).front();

        // prepare buffer write        
        vk::DescriptorBufferInfo bufferInfo = _scalarBuffer->getDescriptorInfo();
        std::vector<vk::WriteDescriptorSet> writes = 
            { _scalarBuffer->getDescriptorWrite(_descriptorSet, 0, bufferInfo) };
        
        // prepare image writes
        std::vector<vk::DescriptorImageInfo> imageInfos(_matrices.size());
        for (int i = 0; i < _matrices.size(); i++)
        {
            imageInfos[i] = _matrices[i]->getDescriptorInfo();
            writes.push_back(_matrices[i]->getDescriptorWrite(_descriptorSet, i+1, imageInfos[i]));
        }

        VKB::device().updateDescriptorSets(writes, {});
    }

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
    {
        StagingBuffer stage(_scalarBuffer->size());
        size_t pos = 0;

        for (auto s : _scalars)
        {
            memcpy((void*)stage + pos, s->value(), sizeofType(s->type()));
            pos += sizeofType(s->type());
        }

        vk::BufferCopy copy; copy
            .setSize(_scalarBuffer->size())
            .setSrcOffset(0)
            .setDstOffset(0);

        auto cmd = VKB::startOneTimeCommandBuffer();
        cmd.copyBuffer(stage.buffer(), _scalarBuffer->buffer(), copy);
        VKB::endOneTimeCommandBuffer(cmd);
    }

    std::vector<std::shared_ptr<MatrixSubres>>& QueueBase::matrices()
    {
        return _matrices;
    }

    std::vector<std::shared_ptr<ScalarSubres>>& QueueBase::scalars()
    {
        return _scalars;
    }

};
