//
// Created by adam on 12.04.2021.
//

#pragma once

#include "./Sokar3D.hpp"
#include "./PipelineWrapper.hpp"
#include "./SimpleLight.hpp"
#include "./SolidMaterial.hpp"

namespace Sokar3D {

    struct VertUniformBufferObject {
        glm::mat4 model = glm::mat4(1);
        glm::mat4 camera;
        glm::mat4 proj;

        static inline VkDeviceSize size() {
//			return sizeof(VertUniformBufferObject);
            return 256;
        }
    };

    struct FragUniformBufferObject {
        glm::vec3 cameraPos;
        SimpleLight light;
        SolidMaterial material;

        static inline VkDeviceSize size() {
//			return sizeof(FragUniformBufferObject);
            return 512;
        }
    };

    class MeshPipeline : public PipelineWrapper {
        QSharedPointer<const StaticMesh> staticMesh;
        VulkanShader vertexShader;
        VulkanShader fragmentShader;
        VkBuffer vertexBuf = VK_NULL_HANDLE;
        VkBuffer indexBuf = VK_NULL_HANDLE;
        VkBuffer uniformBuf = VK_NULL_HANDLE;
        VkBuffer instanceBuf = VK_NULL_HANDLE;
        VkDeviceMemory bufMem = VK_NULL_HANDLE;
        VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
        VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
        VkDeviceSize vertexMemOffset = 0;
        VkDeviceSize indexMemOffset = 0;
        VkDeviceSize uniformMemOffset = 0;
        VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;

        VertUniformBufferObject vertUniformBufferObject;
        FragUniformBufferObject fragUniformBufferObject;

        std::atomic<bool> visible = true;

    public:
        explicit MeshPipeline(const QSharedPointer<const StaticMesh> &mesh);

        void initResources(const VkPipelineMetaArgs &args) override;
        void ensureBuffers(const VkPipelineMetaArgs &args) override;
        void buildDrawCalls(const VkPipelineMetaArgs &args) override;
        void releaseResources(const VkPipelineMetaArgs &args) override;

        void setModelMatrix(const glm::mat4 &model);

        const SolidMaterial &getMeshMaterial() const;
        void setMeshMaterial(const SolidMaterial &material);

        const std::atomic<bool> &isVisible() const;
        void setVisible(bool visible);
    };
}


