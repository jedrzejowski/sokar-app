//
// Created by adam on 12.04.2021.
//

#include <QVulkanDeviceFunctions>
#include "../lib/concat_array.h"
#include "./MeshPipeline.h"
#include "./VulkanWidget.h"
#include "./Camera.h"

using namespace Sokar3D;

MeshPipeline::MeshPipeline(Mesh *mesh) : mesh(mesh) {
	//	backgroundMaterial.model.translate(0, -5, 0);
}

void MeshPipeline::initResources(VkPipelineMetaArgs &args) {
	if (!vertexShader.isValid())
		vertexShader.load(args.vkInstance, args.vkDevice, QStringLiteral(":/vk-shader/mesh.vert.spv"));
	if (!fragmentShader.isValid())
		fragmentShader.load(args.vkInstance, args.vkDevice, QStringLiteral(":/vk-shader/mesh.frag.spv"));
}

void MeshPipeline::releaseResources(VkPipelineMetaArgs &args) {
	if (vertexShader.isValid()) {
		args.vkDeviceFunctions->vkDestroyShaderModule(
				args.vkDevice, vertexShader.data()->shaderModule, nullptr);
		vertexShader.reset();
	}

	if (fragmentShader.isValid()) {
		args.vkDeviceFunctions->vkDestroyShaderModule(
				args.vkDevice, fragmentShader.data()->shaderModule, nullptr);
		fragmentShader.reset();
	}
}

void MeshPipeline::createVkPipeline(VkPipelineMetaArgs &args) {
	VkResult err;

	std::array<VkVertexInputBindingDescription, 1> vertexBindingDesc{
			{
					{
							0,
							sizeof(MeshVertex),
							VK_VERTEX_INPUT_RATE_VERTEX
					}
			}
	};
	auto vertexAttrDesc = concat_array(MeshVertex::getAttributeDescriptions());

	std::array<VkPushConstantRange, 2> pushConstantDesc{
			{
					{
							VK_SHADER_STAGE_VERTEX_BIT,
							0,
							64
					},
					{
							VK_SHADER_STAGE_FRAGMENT_BIT,
							64,
							12
					}
			}};

	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{
			{
					{
							VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
							nullptr,
							0,
							VK_SHADER_STAGE_VERTEX_BIT,
							vertexShader.data()->shaderModule,
							"main",
							nullptr
					},
					{
							VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
							nullptr,
							0,
							VK_SHADER_STAGE_FRAGMENT_BIT,
							fragmentShader.data()->shaderModule,
							"main",
							nullptr
					}
			}};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = vertexBindingDesc.size();
	vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc.data();
	vertexInputInfo.vertexAttributeDescriptionCount = vertexAttrDesc.size();
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc.data();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pushConstantRangeCount = pushConstantDesc.size();
	pipelineLayoutInfo.pPushConstantRanges = pushConstantDesc.data();

	err = args.vkDeviceFunctions->vkCreatePipelineLayout(
			args.vkDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout);
	if (err != VK_SUCCESS)
		qFatal("Failed to create pipeline layout: %d", err);

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputInfo;

	VkPipelineInputAssemblyStateCreateInfo ia{};
	ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipelineInfo.pInputAssemblyState = &ia;

	VkPipelineViewportStateCreateInfo vp{};
	vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vp.viewportCount = 1;
	vp.scissorCount = 1;
	pipelineInfo.pViewportState = &vp;

	VkPipelineRasterizationStateCreateInfo rs{};
	rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rs.polygonMode = VK_POLYGON_MODE_FILL;
	rs.cullMode = VK_CULL_MODE_BACK_BIT;
	rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rs.lineWidth = 1.0f;
	pipelineInfo.pRasterizationState = &rs;

	VkPipelineMultisampleStateCreateInfo ms{};
	ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	ms.rasterizationSamples = args.vkWidget->sampleCountFlagBits();
	pipelineInfo.pMultisampleState = &ms;

	VkPipelineDepthStencilStateCreateInfo ds{};
	ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	ds.depthTestEnable = VK_TRUE;
	ds.depthWriteEnable = VK_TRUE;
	ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	pipelineInfo.pDepthStencilState = &ds;

	VkPipelineColorBlendStateCreateInfo cb{};
	cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	VkPipelineColorBlendAttachmentState att{};
	att.colorWriteMask = 0xF;
	cb.attachmentCount = 1;
	cb.pAttachments = &att;
	pipelineInfo.pColorBlendState = &cb;

	VkDynamicState dynEnable[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dyn{};
	dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
	dyn.pDynamicStates = dynEnable;
	pipelineInfo.pDynamicState = &dyn;

	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = args.vkWidget->defaultRenderPass();

	err = args.vkDeviceFunctions->vkCreateGraphicsPipelines(
			args.vkDevice, args.vkPipelineCache, 1, &pipelineInfo, nullptr, &pipeline);
	if (err != VK_SUCCESS)
		qFatal("Failed to create graphics pipeline: %d", err);
}

void MeshPipeline::ensureBuffers(VkPipelineMetaArgs &args) {
	if (buffersDone) {
		return;
	}
	qDebug("buffersDone: ensureBuffers");
	qDebug() << "initing buffers with " << mesh->data()->sizeInBytes();
	buffersDone = true;

	const int concurrentFrameCount = args.vkWidget->concurrentFrameCount();
	VkResult err;

	VkBufferCreateInfo bufInfo{};

	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.size = mesh->data()->sizeInBytes();
	bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	err = args.vkDeviceFunctions->vkCreateBuffer(args.vkDevice, &bufInfo, nullptr, &vertexBuf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create vertex buffer: %d", err);

	VkMemoryRequirements backgroundVertMemReq;
	args.vkDeviceFunctions->vkGetBufferMemoryRequirements(args.vkDevice, vertexBuf, &backgroundVertMemReq);

	VkMemoryAllocateInfo memAllocInfo = {
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			nullptr,
			static_cast<VkDeviceSize>(mesh->data()->sizeInBytes()),
			args.vkWidget->hostVisibleMemoryIndex()
	};
	err = args.vkDeviceFunctions->vkAllocateMemory(args.vkDevice, &memAllocInfo, nullptr, &bufMem);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate memory: %d", err);

	err = args.vkDeviceFunctions->vkBindBufferMemory(args.vkDevice, vertexBuf, bufMem, 0);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind vertex buffer memory: %d", err);

	// kopiowanie

	quint8 *p;
	err = args.vkDeviceFunctions->vkMapMemory(
			args.vkDevice, bufMem, 0, mesh->data()->sizeInBytes(), 0, reinterpret_cast<void **>(&p));
	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);
	qDebug() << "kopiowanie" << mesh->data()->sizeInBytes();
	memcpy(p, mesh->data()->geom.data(), mesh->data()->sizeInBytes());
	args.vkDeviceFunctions->vkUnmapMemory(args.vkDevice, bufMem);
}

void MeshPipeline::buildDrawCalls(VkPipelineMetaArgs &args) {
	VkCommandBuffer cb = args.vkWidget->currentCommandBuffer();

	args.vkDeviceFunctions->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	VkDeviceSize vbOffset = 0;
	args.vkDeviceFunctions->vkCmdBindVertexBuffers(cb, 0, 1, &vertexBuf, &vbOffset);

	meshConstants.model = meshModel;
	if (!args.camera) {
		qFatal("Camera not set");
	}
	meshConstants.camera = args.camera->viewMatrix();
	meshConstants.proj = args.projectionMatrix;
	meshConstants.color = glm::vec3(1.0f, 0.0f, 0.2f);

	args.vkDeviceFunctions->vkCmdPushConstants(
			cb, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(MeshConstants), &meshConstants);
	args.vkDeviceFunctions->vkCmdPushConstants(
			cb, pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(MeshConstants), &meshConstants);

	args.vkDeviceFunctions->vkCmdDraw(cb, mesh->data()->geom.size(), 1, 0, 0);
}
