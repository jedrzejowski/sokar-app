//
// Created by adam on 29.03.2021.
//

#pragma once

#include "./_def.h"
#include "./VulkanWidget.h"
#include "./Shader.h"
#include "./Mesh.h"
#include "./Camera.h"
#include "./PipelineWrapper.h"

namespace Sokar3D {
	class VulkanRenderer : public QVulkanWindowRenderer {

		QVector<PipelineWrapper *> pipelineWrappers;

		VulkanWidget *vkWidget;
		QVulkanDeviceFunctions *vkDeviceFunctions;
		VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

		QFutureWatcher<void> frameWatcher;
		Camera *camera = nullptr;
		glm::vec3 m_lightPos;
		glm::mat4 projectionMatrix;

		bool framePending;

		void buildFrame();
		void ensureBuffers();
		void buildDrawCalls();
		VkPipelineMetaArgs getMetaArgs();

	public:
		explicit VulkanRenderer(VulkanWidget *w);

		void preInitResources() override;
		void initResources() override;
		void initSwapChainResources() override;
		void releaseSwapChainResources() override;
		void releaseResources() override;
		void startNextFrame() override;

		void addPipelineWrapper(PipelineWrapper *pw);

		Camera *getCamera() const;
		void setCamera(Camera *camera);

		bool uiEvent(QEvent *event);
	};
}


