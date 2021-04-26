//
// Created by adam on 12.04.2021.
//

#pragma once

#include "./_def.h"
#include "./PipelineWrapper.h"

namespace Sokar3D {
	struct MeshConstants {
		glm::mat4 model;
		glm::mat4 camera;
		glm::mat4 proj;
		glm::vec3 color;
	};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 camera;
		glm::mat4 proj;
		glm::vec3 color;

		glm::vec3 lightPos;
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
		VkDescriptorSetLayout descSetLayout = VK_NULL_HANDLE;
		VkDeviceSize vertexMemOffset = VK_NULL_HANDLE;
		VkDeviceSize uniformMemOffset = VK_NULL_HANDLE;
		VkDescriptorSet descSet = VK_NULL_HANDLE;
		VkDeviceSize uniformBufferObjectSize = sizeof(UniformBufferObject);
		UniformBufferObject uniformBufferObject;
		glm::mat4 meshModel = glm::mat4(1);
		bool buffersDone = false;

	public:
		explicit MeshPipeline(Mesh *mesh);

		void initResources(VkPipelineMetaArgs &args) override;
		void createVkPipeline(VkPipelineMetaArgs &args) override;
		void ensureBuffers(VkPipelineMetaArgs &args) override;
		void buildDrawCalls(VkPipelineMetaArgs &args) override;
		void releaseResources(VkPipelineMetaArgs &args) override;
	};
}


