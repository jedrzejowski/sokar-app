//
// Created by adam on 29.03.2021.
//

#pragma once

#include "_def.h"

/**
 * @see https://vulkan-tutorial.com/code/26_depth_buffering.cpp
 */
namespace Sokar3D {

	struct MeshVertex {
		glm::vec3 pos;
		glm::vec2 tex;
		glm::vec3 normal;

		static VkVertexInputBindingDescription getBindingDescription();

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};
}


