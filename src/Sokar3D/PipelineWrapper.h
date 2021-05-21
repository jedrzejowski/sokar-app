//
// Created by adam on 11.04.2021.
//

#pragma once

#include "./Sokar3D.hpp"
#include "./StaticMesh.hpp"
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
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
		VkPipeline vkPipeline = VK_NULL_HANDLE;

	public:
		PipelineWrapper();
		virtual ~PipelineWrapper();

		virtual void initResources(const VkPipelineMetaArgs &args) = 0;
		virtual void createVkPipeline(const VkPipelineMetaArgs &args) = 0;
		virtual void ensureBuffers(const VkPipelineMetaArgs &args) = 0;
		virtual void buildDrawCalls(const VkPipelineMetaArgs &args) = 0;
		virtual void releaseResources(const VkPipelineMetaArgs &args) = 0;
	};
}

