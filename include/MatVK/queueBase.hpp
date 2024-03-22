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

        std::vector<std::shared_ptr<MatrixSubres>>& matrices();
        std::vector<std::shared_ptr<ScalarSubres>>& scalars();

    private:
        std::vector<Shader>         _shaders;
        std::vector<std::shared_ptr<MatrixSubres>> _matrices;
        std::vector<std::shared_ptr<ScalarSubres>> _scalars;

        std::unique_ptr<Buffer>     _scalarBuffer;
        vk::CommandBuffer           _commandBuffer;
        vk::DescriptorSetLayout     _descriptorSetLayout;
        vk::DescriptorPool          _descriptorPool;
        vk::DescriptorSet           _descriptorSet;
        vk::PipelineLayout          _pipelineLayout;
        std::vector<vk::Pipeline>   _pipelines;
        vk::Fence                   _fence;

        void createScalarBuffer();
        void createDescriptorSetLayout();
        void createDescriptorPool();
        void createDescriptorSet();
        void createPipelines();
        void createCommandBuffer();
        void updateUniformBuffer();
    };

};
