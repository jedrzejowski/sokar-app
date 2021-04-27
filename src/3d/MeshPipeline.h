//
// Created by adam on 12.04.2021.
//

#pragma once

#include "./_def.h"
#include "./PipelineWrapper.h"
#include "./SimpleLight.h"
#include "./SolidMaterial.h"

namespace Sokar3D {
	struct MeshConstants {
		glm::mat4 model;
		glm::mat4 camera;
		glm::mat4 proj;
		glm::vec3 color;
	};

	struct VertUniformBufferObject {
		glm::mat4 model;
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
		Mesh *mesh;
		Shader vertexShader;
		Shader fragmentShader;
		VkBuffer vertexBuf = VK_NULL_HANDLE;
		VkBuffer uniformBuf = VK_NULL_HANDLE;
		VkBuffer instanceBuf = VK_NULL_HANDLE;
		VkDeviceMemory bufMem = VK_NULL_HANDLE;
		VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
		VkDeviceSize vertexMemOffset = VK_NULL_HANDLE;
		VkDeviceSize uniformMemOffset = VK_NULL_HANDLE;
		VkDescriptorSet vkDescriptorSet = VK_NULL_HANDLE;

		VertUniformBufferObject vertUniformBufferObject;
		FragUniformBufferObject fragUniformBufferObject;

		glm::mat4 meshModel = glm::mat4(1);

	public:
		explicit MeshPipeline(Mesh *mesh);

		void initResources(const VkPipelineMetaArgs &args) override;
		void createVkPipeline(const VkPipelineMetaArgs &args) override;
		void ensureBuffers(const VkPipelineMetaArgs &args) override;
		void buildDrawCalls(const VkPipelineMetaArgs &args) override;
		void releaseResources(const VkPipelineMetaArgs &args) override;
	};
}


