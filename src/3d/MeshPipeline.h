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

	class MeshPipeline : public PipelineWrapper {
		Mesh *mesh;
		Shader vertexShader;
		Shader fragmentShader;
		VkBuffer vertexBuf = VK_NULL_HANDLE;
		VkBuffer instanceBuf = VK_NULL_HANDLE;
		VkDeviceMemory bufMem = VK_NULL_HANDLE;
		MeshConstants meshConstants;
		glm::mat4 meshModel = glm::mat4(1);
		bool buffersDone = false;

	public:
		explicit MeshPipeline(Mesh *mesh);

		void initResources(VkPipelineMetaArgs &args) override;
		void createDescriptorSetLayout();
		void createVkPipeline(VkPipelineMetaArgs &args) override;
		void ensureBuffers(VkPipelineMetaArgs &args) override;
		void buildDrawCalls(VkPipelineMetaArgs &args) override;
		void releaseResources(VkPipelineMetaArgs &args) override;
	};
}


