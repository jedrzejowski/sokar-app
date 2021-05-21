//
// Created by adam on 12.04.2021.
//

#include <QVulkanDeviceFunctions>
#include "../lib/concat_array.h"
#include "./MeshPipeline.hpp"
#include "./VulkanWidget.hpp"
#include "./Camera.hpp"

using namespace Sokar3D;

MeshPipeline::MeshPipeline(
		const QSharedPointer<const StaticMesh> &mesh
) : staticMesh(mesh) {
}

void MeshPipeline::releaseResources(const VkPipelineMetaArgs &args) {

	if (vkDescriptorPool) {
		args.vkDeviceFunctions->vkDestroyDescriptorPool(args.vkDevice, vkDescriptorPool, nullptr);
		vkDescriptorPool = VK_NULL_HANDLE;
	}

	if (vkDescriptorSetLayout) {
		args.vkDeviceFunctions->vkDestroyDescriptorSetLayout(args.vkDevice, vkDescriptorSetLayout, nullptr);
		vkDescriptorSetLayout = VK_NULL_HANDLE;
	}


	if (vkPipeline) {
		args.vkDeviceFunctions->vkDestroyPipeline(args.vkDevice, vkPipeline, nullptr);
		vkPipeline = VK_NULL_HANDLE;
	}

	if (vkPipelineLayout) {
		args.vkDeviceFunctions->vkDestroyPipelineLayout(args.vkDevice, vkPipelineLayout, nullptr);
		vkPipelineLayout = VK_NULL_HANDLE;
	}

	if (vertexBuf) {
		args.vkDeviceFunctions->vkDestroyBuffer(args.vkDevice, vertexBuf, nullptr);
		vertexBuf = VK_NULL_HANDLE;
	}

	if (indexBuf) {
		args.vkDeviceFunctions->vkDestroyBuffer(args.vkDevice, indexBuf, nullptr);
		indexBuf = VK_NULL_HANDLE;
	}

	if (uniformBuf) {
		args.vkDeviceFunctions->vkDestroyBuffer(args.vkDevice, uniformBuf, nullptr);
		uniformBuf = VK_NULL_HANDLE;
	}

	if (instanceBuf) {
		args.vkDeviceFunctions->vkDestroyBuffer(args.vkDevice, instanceBuf, nullptr);
		instanceBuf = VK_NULL_HANDLE;
	}

	if (bufMem) {
		args.vkDeviceFunctions->vkFreeMemory(args.vkDevice, bufMem, nullptr);
		bufMem = VK_NULL_HANDLE;
	}

	if (vertexShader.isValid()) {
		args.vkDeviceFunctions->vkDestroyShaderModule(
				args.vkDevice, vertexShader.getShaderModule(), nullptr);
		vertexShader.reset();
	}

	if (fragmentShader.isValid()) {
		args.vkDeviceFunctions->vkDestroyShaderModule(
				args.vkDevice, fragmentShader.getShaderModule(), nullptr);
		fragmentShader.reset();
	}
}

void MeshPipeline::initResources(const VkPipelineMetaArgs &args) {
	if (vkPipeline) {
		return;
	}

	VkResult err;

	const VkPhysicalDeviceLimits *pdevLimits = &args.vkWidget->physicalDeviceProperties()->limits;
	const VkDeviceSize uniAlign = pdevLimits->minUniformBufferOffsetAlignment;

	if (!vertexShader.isValid()) {
		vertexShader.load(args.vkInstance, args.vkDevice, QStringLiteral(":/Sokar3D/StaticMesh.vert"));
	}
	if (!fragmentShader.isValid()) {
		fragmentShader.load(args.vkInstance, args.vkDevice, QStringLiteral(":/Sokar3D/StaticMesh.frag"));
	}

	std::vector<VkVertexInputBindingDescription> vertexBindingDesc = {
			{
					0,
					sizeof(MeshVertex),
					VK_VERTEX_INPUT_RATE_VERTEX
			}
	};

	auto vertexAttrDesc = MeshVertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.pNext = nullptr;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = vertexBindingDesc.size();
	vertexInputInfo.pVertexBindingDescriptions = vertexBindingDesc.data();
	vertexInputInfo.vertexAttributeDescriptionCount = vertexAttrDesc.size();
	vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc.data();

	// tworzenie deskryptorów dla uniform bufferów
	std::vector<VkDescriptorPoolSize> descPoolSizes = {
			{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, static_cast<uint32_t>(args.vkWidget->swapChainImageCount())}
	};
	VkDescriptorPoolCreateInfo descPoolInfo{};
	descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descPoolInfo.maxSets = 1;
	descPoolInfo.poolSizeCount = descPoolSizes.size();
	descPoolInfo.pPoolSizes = descPoolSizes.data();
	err = args.vkDeviceFunctions->vkCreateDescriptorPool(args.vkDevice, &descPoolInfo, nullptr, &vkDescriptorPool);
	if (err != VK_SUCCESS)
		qFatal("Failed to create descriptor pool: %d", err);

	std::vector<VkDescriptorSetLayoutBinding> layoutBindings = {
			{
					0,
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
					1,
					VK_SHADER_STAGE_VERTEX_BIT,
					nullptr
			},
			{
					1,
					VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
					1,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					nullptr
			}
	};
	VkDescriptorSetLayoutCreateInfo descLayoutInfo{};
	descLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descLayoutInfo.pNext = nullptr;
	descLayoutInfo.flags = 0;
	descLayoutInfo.bindingCount = layoutBindings.size();
	descLayoutInfo.pBindings = layoutBindings.data();
	err = args.vkDeviceFunctions->vkCreateDescriptorSetLayout(args.vkDevice, &descLayoutInfo, nullptr,
															  &vkDescriptorSetLayout);
	if (err != VK_SUCCESS)
		qFatal("Failed to create descriptor set layout: %d", err);

	VkDescriptorSetAllocateInfo descSetAllocInfo{};
	descSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descSetAllocInfo.pNext = nullptr;
	descSetAllocInfo.descriptorPool = vkDescriptorPool;
	descSetAllocInfo.descriptorSetCount = descPoolSizes.size();
	descSetAllocInfo.pSetLayouts = &vkDescriptorSetLayout;
	err = args.vkDeviceFunctions->vkAllocateDescriptorSets(args.vkDevice, &descSetAllocInfo, &vkDescriptorSet);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate descriptor set: %d", err);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pSetLayouts = &vkDescriptorSetLayout;
	pipelineLayoutInfo.setLayoutCount = 1;

	err = args.vkDeviceFunctions->vkCreatePipelineLayout(
			args.vkDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout);
	if (err != VK_SUCCESS)
		qFatal("Failed to create pipeline layout: %d", err);

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {
			{
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					nullptr,
					0,
					VK_SHADER_STAGE_VERTEX_BIT,
					vertexShader.getShaderModule(),
					"main",
					nullptr
			},
			{
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					nullptr,
					0,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					fragmentShader.getShaderModule(),
					"main",
					nullptr
			}
	};

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
	rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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

	pipelineInfo.layout = vkPipelineLayout;
	pipelineInfo.renderPass = args.vkWidget->defaultRenderPass();

	err = args.vkDeviceFunctions->vkCreateGraphicsPipelines(
			args.vkDevice, args.vkPipelineCache, 1, &pipelineInfo, nullptr, &vkPipeline);
	if (err != VK_SUCCESS)
		qFatal("Failed to create graphics pipeline: %d", err);
}

void MeshPipeline::ensureBuffers(const VkPipelineMetaArgs &args) {
	initResources(args);

	if (vertexBuf) {
		return;
	}

	qDebug("buffersDone: ensureBuffers");

	const int concurrentFrameCount = args.vkWidget->concurrentFrameCount();
	VkResult err;


	// Tworzenie bufforów


	// vetrex buffor
	VkBufferCreateInfo bufInfo{};
	bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufInfo.pNext = nullptr;
	bufInfo.size = staticMesh->verticesSizeInBytes();
	bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	err = args.vkDeviceFunctions->vkCreateBuffer(args.vkDevice, &bufInfo, nullptr, &vertexBuf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create vertex buffer: %d", err);
	VkMemoryRequirements vertexMemReq;
	args.vkDeviceFunctions->vkGetBufferMemoryRequirements(args.vkDevice, vertexBuf, &vertexMemReq);

	// index buffer
//	bufInfo.size = indexedStaticMesh != nullptr ? indexedStaticMesh->indexesSizeInBytes() : 0;
	bufInfo.size = 0;
	bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	err = args.vkDeviceFunctions->vkCreateBuffer(args.vkDevice, &bufInfo, nullptr, &indexBuf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create index buffer: %d", err);
	VkMemoryRequirements indexMemReq;
	args.vkDeviceFunctions->vkGetBufferMemoryRequirements(args.vkDevice, indexBuf, &indexMemReq);

	// uniform buffer
	bufInfo.size = (VertUniformBufferObject::size() + FragUniformBufferObject::size()) * concurrentFrameCount;
	bufInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	err = args.vkDeviceFunctions->vkCreateBuffer(args.vkDevice, &bufInfo, nullptr, &uniformBuf);
	if (err != VK_SUCCESS)
		qFatal("Failed to create uniform buffer: %d", err);
	VkMemoryRequirements uniformMemReq;
	args.vkDeviceFunctions->vkGetBufferMemoryRequirements(args.vkDevice, uniformBuf, &uniformMemReq);

	// alokowanie pamięcie

	VkMemoryAllocateInfo memAllocInfo{};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = nullptr;
	memAllocInfo.allocationSize =
			(VertUniformBufferObject::size() + FragUniformBufferObject::size()) * concurrentFrameCount +
			0 + // (indexedStaticMesh != nullptr ? indexedStaticMesh->indexesSizeInBytes() : 0) +
			static_cast<VkDeviceSize>(staticMesh->verticesSizeInBytes());
	memAllocInfo.memoryTypeIndex = args.vkWidget->hostVisibleMemoryIndex();

	err = args.vkDeviceFunctions->vkAllocateMemory(args.vkDevice, &memAllocInfo, nullptr, &bufMem);
	if (err != VK_SUCCESS)
		qFatal("Failed to allocate memory: %d", err);

	// przpypisywanie bufforów do pamięci

	vertexMemOffset = 0;
	indexMemOffset = makeBuffSizeAligned(vertexMemOffset + vertexMemReq.size, indexMemReq.alignment);
	uniformMemOffset = makeBuffSizeAligned(indexMemOffset + indexMemReq.size, uniformMemReq.alignment);

	err = args.vkDeviceFunctions->vkBindBufferMemory(args.vkDevice, vertexBuf, bufMem, vertexMemOffset);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind vertex buffer memory: %d", err);
	err = args.vkDeviceFunctions->vkBindBufferMemory(args.vkDevice, indexBuf, bufMem, indexMemOffset);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind index buffer memory: %d", err);
	err = args.vkDeviceFunctions->vkBindBufferMemory(args.vkDevice, uniformBuf, bufMem, uniformMemOffset);
	if (err != VK_SUCCESS)
		qFatal("Failed to bind uniform buffer memory: %d", err);

	// kopiowanie danych

	quint8 *p;
	err = args.vkDeviceFunctions->vkMapMemory(
			args.vkDevice, bufMem, 0, staticMesh->verticesSizeInBytes(), 0, reinterpret_cast<void **>(&p));
	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);
	memcpy(p, staticMesh->verticesData(), staticMesh->verticesSizeInBytes());
//	if (indexedStaticMesh) {
//		memcpy(p + indexMemOffset, indexedStaticMesh->indexData(), indexedStaticMesh->indexesSizeInBytes());
//	}
	args.vkDeviceFunctions->vkUnmapMemory(args.vkDevice, bufMem);

	// deskryptry

	VkDescriptorBufferInfo vertUniformDescBuffInfo = {uniformBuf, 0, VertUniformBufferObject::size()};
	VkDescriptorBufferInfo fragUniformDescBuffInfo = {uniformBuf, VertUniformBufferObject::size(),
													  FragUniformBufferObject::size()};

	std::vector<VkWriteDescriptorSet> descWrite(2);

	descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descWrite[0].dstSet = vkDescriptorSet;
	descWrite[0].dstBinding = 0;
	descWrite[0].descriptorCount = 1;
	descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	descWrite[0].pBufferInfo = &vertUniformDescBuffInfo;

	descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descWrite[1].dstSet = vkDescriptorSet;
	descWrite[1].dstBinding = 1;
	descWrite[1].descriptorCount = 1;
	descWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
	descWrite[1].pBufferInfo = &fragUniformDescBuffInfo;

	args.vkDeviceFunctions->vkUpdateDescriptorSets(args.vkDevice, descWrite.size(), descWrite.data(), 0, nullptr);
}

void MeshPipeline::buildDrawCalls(const VkPipelineMetaArgs &args) {
	// if pipeline is still not exists
	if (!vkPipeline) {
		return;
	}


	VkCommandBuffer cb = args.vkWidget->currentCommandBuffer();
	VkResult err;

	vertUniformBufferObject.camera = args.camera->viewMatrix();
	vertUniformBufferObject.proj = args.projectionMatrix;

	fragUniformBufferObject.cameraPos = args.camera->position();

	fragUniformBufferObject.light.position = args.camera->position() + glm::vec3(0.0f, 2.0f, 0.0f);
	fragUniformBufferObject.light.ambient = glm::vec3(0.4f);
	fragUniformBufferObject.light.diffuse = glm::vec3(0.5f);
	fragUniformBufferObject.light.specular = glm::vec3(1.0f);

	fragUniformBufferObject.material.shininess = 64.0f;
	fragUniformBufferObject.material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	fragUniformBufferObject.material.color = glm::vec3(0.0f, 1.f, 0.f);

	args.vkDeviceFunctions->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

	VkDeviceSize vbOffset = 0, ibOffset = 0;
	args.vkDeviceFunctions->vkCmdBindVertexBuffers(cb, 0, 1, &vertexBuf, &vbOffset);
//	if (indexedStaticMesh) {
//		args.vkDeviceFunctions->vkCmdBindIndexBuffer(cb, indexBuf, ibOffset, VK_INDEX_TYPE_UINT32);
//	}

	uint32_t frameUniOffset =
			args.vkWidget->currentFrame() * (VertUniformBufferObject::size() + FragUniformBufferObject::size());
	uint32_t frameUniOffsets[] = {frameUniOffset, frameUniOffset};
	args.vkDeviceFunctions->vkCmdBindDescriptorSets(
			cb, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout,
			0, 1,
			&vkDescriptorSet,
			2,
			frameUniOffsets);

	quint8 *p;
	err = args.vkDeviceFunctions->vkMapMemory(
			args.vkDevice, bufMem, uniformMemOffset + frameUniOffset,
			VertUniformBufferObject::size(), 0, reinterpret_cast<void **>(&p));
	if (err != VK_SUCCESS)
		qFatal("Failed to map memory: %d", err);
	memcpy(p, &vertUniformBufferObject, VertUniformBufferObject::size());
	memcpy(p + VertUniformBufferObject::size(), &fragUniformBufferObject, FragUniformBufferObject::size());
	args.vkDeviceFunctions->vkUnmapMemory(args.vkDevice, bufMem);

//	if (indexedStaticMesh) {
//		args.vkDeviceFunctions->vkCmdDrawIndexed(cb, indexedStaticMesh->indexCount(), 1, 0, 0, 0);
//	} else {
	args.vkDeviceFunctions->vkCmdDraw(cb, staticMesh->verticesCount(), 1, 0, 0);
//	}
}


void MeshPipeline::setModelMatrix(const glm::mat4 &model) {
	vertUniformBufferObject.model = model;
}