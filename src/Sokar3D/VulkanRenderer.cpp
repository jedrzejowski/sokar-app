//
// Created by adam on 29.03.2021.
//

#include "./VulkanRenderer.hpp"
#include "qrandom.h"
#include <QVulkanFunctions>
#include <QtConcurrent/QtConcurrentRun>
#include <QTime>
#include <QMouseEvent>
#include "SokarLib/concat_array.h"
#include "./PipelineWrapper.hpp"

using namespace Sokar3D;

#define DBG Q_UNLIKELY(m_window->isDebugEnabled())

VulkanRenderer::VulkanRenderer(
		VulkanWidget *widget
) : vkWidget(widget),
	m_lightPos(0.0f, 0.0f, 25.0f) {

//	backgroundMaterial.model.translate(0, -5, 0);
//	backgroundMaterial.model.rotate(-90, 1, 0, 0);
//	backgroundMaterial.model.scale(20, 100, 1);

//	backgroundMaterial.mesh.addTriangle(
//			Vertex{
//					vec3{-1, -1, 0}
//			},
//			Vertex{
//					vec3{-1, 1, 0}
//			},
//			Vertex{
//					vec3{1, -1, 1}
//			}
//	);

	QObject::connect(&frameWatcher, &QFutureWatcherBase::finished, [this] {
		if (framePending) {
			framePending = false;
			vkWidget->frameReady();
			vkWidget->requestUpdate();
		}
	});
}

void VulkanRenderer::preInitResources() {
	qDebug("preInitResources");
}

void VulkanRenderer::initResources() {
	qDebug("initResources");
	VkDevice vkDevice = vkWidget->device();

	QVulkanInstance *vkInstance = vkWidget->vulkanInstance();
	vkDeviceFunctions = vkInstance->deviceFunctions(vkDevice);

	VkPipelineCacheCreateInfo pipelineCacheInfo{};
	pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	VkResult err = vkDeviceFunctions->vkCreatePipelineCache(vkDevice, &pipelineCacheInfo, nullptr, &vkPipelineCache);
	if (err != VK_SUCCESS)
		qFatal("Failed to create pipeline cache: %d", err);

	VkPipelineMetaArgs args{};
	args.vkInstance = vkInstance;
	args.vkDevice = vkDevice;
	args.vkWidget = vkWidget;
	args.vkDeviceFunctions = vkDeviceFunctions;
	args.vkPipelineCache = vkPipelineCache;
	args.projectionMatrix = projectionMatrix;
	args.camera = camera;

	initResourceFuture = QtConcurrent::run([&, args] {
		auto copy = QVector(pipelineWrappers.current);

		for (auto pw : copy) {
			QMutexLocker locker(&pipelinesMutex);
			pw->initResources(args);
		}
	});
}

void VulkanRenderer::initSwapChainResources() {
	qDebug("initSwapChainResources");

	auto proj = vkWidget->clipCorrectionMatrix();
	const QSize sz = vkWidget->swapChainImageSize();
	proj.perspective(45.0f, sz.width() / (float) sz.height(), 0.01f, 1000000.0f);

	projectionMatrix = glm::make_mat4(proj.data());
}

void VulkanRenderer::releaseSwapChainResources() {
	qDebug("releaseSwapChainResources");

	// tutaj oczekujemy na zakończenie

	frameWatcher.waitForFinished();

	if (framePending) {
		framePending = false;
		vkWidget->frameReady();
	}
}

void VulkanRenderer::releaseResources() {
	initResourceFuture.waitForFinished();
	releaseResourcesFuture.waitForFinished();

	QMutexLocker locker(&pipelinesMutex);

	qDebug("releaseResources");
	VkDevice vkDevice = vkWidget->device();
	auto args = getMetaArgs();

	if (vkPipelineCache) {
		vkDeviceFunctions->vkDestroyPipelineCache(vkDevice, vkPipelineCache, nullptr);
		vkPipelineCache = VK_NULL_HANDLE;
	}

	for (auto pw : pipelineWrappers.current) {
		pw->releaseResources(args);
	}

	pipelineWrappers.current << pipelineWrappers.toAdd;
	pipelineWrappers.toAdd.clear();

	for (auto pw : pipelineWrappers.toRemove) {
		pw->releaseResources(args);
	}
	pipelineWrappers.toRemove.clear();

	for (auto pw : pipelineWrappers.toRelease) {
		pw->releaseResources(args);
	}
	pipelineWrappers.toRemove.clear();
}

void VulkanRenderer::startNextFrame() {
//	qDebug("startNextFrame");

	Q_ASSERT(!framePending);
	framePending = true;

	QMutexLocker locker(&pipelinesMutex);
	auto meta = getMetaArgs();


	while (!pipelineWrappers.toAdd.isEmpty()) {
		auto pipeline = pipelineWrappers.toAdd.takeFirst();
		pipeline->initResources(meta);
		pipelineWrappers.current.append(pipeline);
	}

	while (!releaseResourcesFuture.isRunning() && !pipelineWrappers.toRelease.isEmpty()) {
		auto pipeline = pipelineWrappers.toRelease.takeFirst();
		pipelineWrappers.toRelease.removeOne(pipeline);

		releaseResourcesFuture = QtConcurrent::run([meta, pipeline]() {
			pipeline->releaseResources(meta);
			delete pipeline;
		});
	}

	while (!pipelineWrappers.toRemove.isEmpty()) {
		auto pipeline = pipelineWrappers.toRemove.takeFirst();
		pipelineWrappers.current.removeOne(pipeline);
		pipelineWrappers.toRelease.append(pipeline);
	}


	// tworzymy kopię
	auto pipelines = new Pipelines(pipelineWrappers.current);

	QFuture<void> future = QtConcurrent::run(this, &VulkanRenderer::buildFrame, pipelines);
	frameWatcher.setFuture(future);
}

void VulkanRenderer::buildFrame(Pipelines *pipelines) {

	ensureBuffers(pipelines);
	initResourceFuture.waitForFinished();

	VkCommandBuffer cb = vkWidget->currentCommandBuffer();
	const QSize sz = vkWidget->swapChainImageSize();

	VkClearColorValue clearColor = {{0.67f, 0.84f, 0.9f, 1.0f}};
	VkClearDepthStencilValue clearDS = {1, 0};
	VkClearValue clearValues[3];
	memset(clearValues, 0, sizeof(clearValues));
	clearValues[0].color = clearValues[2].color = clearColor;
	clearValues[1].depthStencil = clearDS;

	VkRenderPassBeginInfo rpBeginInfo{};
	rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBeginInfo.renderPass = vkWidget->defaultRenderPass();
	rpBeginInfo.framebuffer = vkWidget->currentFramebuffer();
	rpBeginInfo.renderArea.extent.width = sz.width();
	rpBeginInfo.renderArea.extent.height = sz.height();
	rpBeginInfo.clearValueCount = vkWidget->sampleCountFlagBits() > VK_SAMPLE_COUNT_1_BIT ? 3 : 2;
	rpBeginInfo.pClearValues = clearValues;
	VkCommandBuffer cmdBuf = vkWidget->currentCommandBuffer();
	vkDeviceFunctions->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport = {
			0, 0,
			float(sz.width()), float(sz.height()),
			0, 1
	};
	vkDeviceFunctions->vkCmdSetViewport(cb, 0, 1, &viewport);

	VkRect2D scissor = {
			{0,                    0},
			{uint32_t(sz.width()), uint32_t(sz.height())}
	};
	vkDeviceFunctions->vkCmdSetScissor(cb, 0, 1, &scissor);

	buildDrawCalls(pipelines);

	vkDeviceFunctions->vkCmdEndRenderPass(cmdBuf);
}

void VulkanRenderer::ensureBuffers(const Pipelines *pipelines) {
	auto args = getMetaArgs();

	for (auto pw : *pipelines) {
		pw->ensureBuffers(args);
	}
}

void VulkanRenderer::buildDrawCalls(const Pipelines *pipelines) {

	auto args = getMetaArgs();

	for (auto pw : *pipelines) {
		if (pw->isHidden()) {
			continue;
		}

		pw->buildDrawCalls(args);
	}
}

Camera *VulkanRenderer::getCamera() const {
	return camera;
}

void VulkanRenderer::setCamera(Camera *newCamera) {
	camera = newCamera;
}

VkPipelineMetaArgs VulkanRenderer::getMetaArgs() {
	VkDevice vkDevice = vkWidget->device();

	VkPipelineMetaArgs args{};

	args.vkInstance = vkWidget->vulkanInstance();
	args.vkDevice = vkDevice;
	args.vkWidget = vkWidget;
	args.vkDeviceFunctions = vkDeviceFunctions;
	args.vkPipelineCache = vkPipelineCache;
	args.projectionMatrix = projectionMatrix;
	args.camera = camera;

	return args;
}

void VulkanRenderer::addPipeline(PipelineWrapper *pw) {
	QMutexLocker locker(&pipelinesMutex);
	pipelineWrappers.toAdd << pw;
}

void VulkanRenderer::removePipeline(PipelineWrapper *pw) {
	QMutexLocker locker(&pipelinesMutex);
	pipelineWrappers.toRemove << pw;
}

//region Ui Events

bool VulkanRenderer::uiEvent(QEvent *event) {

	if (camera && camera->uiEvent(event)) {
		return true;
	}

	return false;
}


//endregion
