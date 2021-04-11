//
// Created by adam on 12.04.2021.
//

#pragma once

#include "./_def.h"
#include "./PipelineWrapper.h"

namespace Sokar3D {
	class MeshPipeline : public PipelineWrapper {
		Mesh mesh;
		Shader vertexShader;
		Shader fragmentShader;
		VkBuffer vertexBuf = VK_NULL_HANDLE;

		QMatrix4x4 model;
	public:
		void initResources(VkPipelineMetaArgs &args) override;
		void createVkPipeline(VkPipelineMetaArgs &args) override;
		void ensureBuffers(VkPipelineMetaArgs &args) override;
		void buildDrawCalls(VkPipelineMetaArgs &args) override;
		void releaseResources(VkPipelineMetaArgs &args) override;
	};
}


