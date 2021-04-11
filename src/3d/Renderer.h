//
// Created by adam on 29.03.2021.
//

#pragma once

#include "./_def.h"
#include "./VulkanWidget.h"
#include "./Shader.h"
#include "./Mesh.h"
#include "./Camera.h"

namespace Sokar3D {
	class Renderer : public QVulkanWindowRenderer {
	public:
		Renderer(VulkanWidget *w, int initialCount = 128);

		void preInitResources() override;
		void initResources() override;
		void initSwapChainResources() override;
		void releaseSwapChainResources() override;
		void releaseResources() override;
		void startNextFrame() override;

	private:

		VulkanWidget *vkWidget;
		QVulkanDeviceFunctions *vkDeviceFunctions;
		VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;


		QFutureWatcher<void> frameWatcher;
		QVector3D m_lightPos;
		Camera m_cam;
		QMatrix4x4 m_proj;

		bool framePending;
		void buildFrame();
		void ensureBuffers();
		void buildDrawCalls();
		bool buffersDone = false;
		VkDeviceMemory bufMem = VK_NULL_HANDLE;

		void createBackgroundPipeline();

		struct {
			Mesh mesh;
			Shader vertexShader;
			Shader fragmentShader;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkPipeline pipeline = VK_NULL_HANDLE;
			VkBuffer vertexBuf = VK_NULL_HANDLE;

			QMatrix4x4 model;
		} backgroundMaterial;
	};
}


