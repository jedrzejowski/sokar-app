//
// Created by adam on 11.04.2021.
//

#pragma once

#include "./_def.h"
#include "./Mesh.h"
#include "./Shader.h"

namespace Sokar3D {

	struct VkPipelineMetaArgs {
		QVulkanInstance *vkInstance;
		VkDevice vkDevice;
		VulkanWidget *vkWidget;
		QVulkanDeviceFunctions *vkDeviceFunctions;
		VkPipelineCache vkPipelineCache;
		glm::mat4 projectionMatrix;
		Camera *camera;
	};

	class PipelineWrapper {
	protected:
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline pipeline = VK_NULL_HANDLE;

	public:
		PipelineWrapper();
		virtual ~PipelineWrapper();

		virtual void initResources(VkPipelineMetaArgs &args) = 0;
		virtual void createVkPipeline(VkPipelineMetaArgs &args) = 0;
		virtual void ensureBuffers(VkPipelineMetaArgs &args) = 0;
		virtual void buildDrawCalls(VkPipelineMetaArgs &args) = 0;
		virtual void releaseResources(VkPipelineMetaArgs &args) = 0;
	};
}

