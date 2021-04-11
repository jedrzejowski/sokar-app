//
// Created by adam on 12.04.2021.
//

#include <QVulkanDeviceFunctions>
#include "lib/concat_array.h"
#include "MeshPipeline.h"
#include "VulkanWidget.h"

using namespace Sokar3D;

void MeshPipeline::initResources(VkPipelineMetaArgs &args) {
	if (!vertexShader.isValid())
		vertexShader.load(args.vkInstance, args.vkDevice, QStringLiteral(":/vk-shader/background.vert.spv"));
	if (!fragmentShader.isValid())
		fragmentShader.load(args.vkInstance, args.vkDevice, QStringLiteral(":/vk-shader/background.frag.spv"));
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
}


void MeshPipeline::createVkPipeline(VkPipelineMetaArgs &args) {
	VkResult err;

	std::array<VkVertexInputBindingDescription, 1> vertexBindingDesc{
			{
					Vertex::getBindingDescription()
			}
	};
	auto vertexAttrDesc = concat_array(Vertex::getAttributeDescriptions());

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
	ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
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

	err =  args.vkDeviceFunctions->vkCreateGraphicsPipelines(
			args.vkDevice,  args.vkPipelineCache, 1, &pipelineInfo, nullptr, &pipeline);
	if (err != VK_SUCCESS)
		qFatal("Failed to create graphics pipeline: %d", err);
}

void MeshPipeline::ensureBuffers(VkPipelineMetaArgs &args) {
}

void MeshPipeline::buildDrawCalls(VkPipelineMetaArgs &args) {
}
