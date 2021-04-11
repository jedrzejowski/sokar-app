//
// Created by adam on 29.03.2021.
//

#pragma once

#include "_def.h"

/**
 * @see https://vulkan-tutorial.com/code/26_depth_buffering.cpp
 */
namespace Sokar3D {

	struct Vertex {
		vec3 pos;
		vec2 tex;
		vec3 color;
		vec3 normal;

		static VkVertexInputBindingDescription getBindingDescription();

		static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions();
	};
}


