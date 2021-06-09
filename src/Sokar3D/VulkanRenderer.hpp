//
// Created by adam on 29.03.2021.
//

#pragma once

#include "./Sokar3D.hpp"
#include "./VulkanWidget.hpp"
#include "./VulkanShader.hpp"
#include "./StaticMesh.hpp"
#include "./Camera.hpp"
#include "./PipelineWrapper.hpp"

namespace Sokar3D {
	class VulkanRenderer : public QVulkanWindowRenderer {

		using Pipelines = QVector<PipelineWrapper *>;
		struct {
			Pipelines toAdd;
			Pipelines current;
			Pipelines toRemove;
			Pipelines toRelease;
		} pipelineWrappers;

		QMutex pipelinesMutex;

		VulkanWidget *vkWidget;
		QVulkanDeviceFunctions *vkDeviceFunctions;
		VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

		QFutureWatcher<void> frameWatcher;
		QFuture<void> initResourceFuture;
		QFuture<void> releaseResourcesFuture;

		Camera *camera = nullptr;
		glm::vec3 m_lightPos;
		glm::mat4 projectionMatrix;

		bool framePending = false;

		void buildFrame(Pipelines *pipelines);
		void ensureBuffers(const Pipelines *pipelines);
		void buildDrawCalls(const Pipelines *pipelines);
		VkPipelineMetaArgs getMetaArgs();

	public:
		explicit VulkanRenderer(VulkanWidget *w);

		void preInitResources() override;
		void initResources() override;
		void initSwapChainResources() override;
		void releaseSwapChainResources() override;
		void releaseResources() override;
		void startNextFrame() override;

		void addPipeline(PipelineWrapper *pw);
		void removePipeline(PipelineWrapper *pw);

		Camera *getCamera() const;
		void setCamera(Camera *camera);

		bool uiEvent(QEvent *event);
	};
}


