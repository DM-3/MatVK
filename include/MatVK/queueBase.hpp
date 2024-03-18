#pragma once

#include "matvk.hpp"
#include "buffer.hpp"
#include "shader.hpp"


namespace matvk
{

    class QueueBase
    {
    public:
        QueueBase();
        ~QueueBase();
        void addAssignment(Assignment assignment);
        void compile();
        void execute();
        void waitFinished();

    private:
        std::vector<Shader>     _shaders;
        std::unique_ptr<Buffer> _uniformBuffer;

        vk::CommandBuffer       _commandBuffer;
        vk::DescriptorSetLayout _descriptorSetLayout;
        vk::DescriptorPool      _descriptorPool;
        vk::DescriptorSet       _descriptorSet;
        vk::PipelineLayout      _pipelineLayout;
        vk::Fence               _fence;

        void createUniformBuffer();
        void createDescriptorSetLayout();
        void createDescriptorPool();
        void createDescriptorSet();
        void createPipelines();
        void createCommandBuffer();
        void updateUniformBuffer();
    };

};
