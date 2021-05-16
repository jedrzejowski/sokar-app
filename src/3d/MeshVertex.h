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
//		glm::vec2 tex;
		glm::vec3 normal;

		static VkVertexInputBindingDescription getBindingDescription();

		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		static bool areEquals(const MeshVertex &v1, const MeshVertex &v2, float esp = EPS);
	};
}


QDebug operator<<(QDebug dbg, const Sokar3D::MeshVertex &tri);

