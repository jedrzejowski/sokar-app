//
// Created by adam on 29.03.2021.
//

#include "./MeshVertex.h"

using namespace Sokar3D;

VkVertexInputBindingDescription MeshVertex::getBindingDescription() {

	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(MeshVertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> MeshVertex::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(MeshVertex, pos);

//	attributeDescriptions[1].binding = 0;
//	attributeDescriptions[1].location = 1;
//	attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
//	attributeDescriptions[1].offset = offsetof(MeshVertex, tex);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(MeshVertex, normal);

	return attributeDescriptions;
}

bool MeshVertex::areEquals(const MeshVertex &v1, const MeshVertex &v2, float esp) {
	return glm::distance(v1.pos, v2.pos) < esp;
}


QDebug operator<<(QDebug dbg, const Sokar3D::MeshVertex &vertex) {

	return dbg
			<< "MeshVertex{"
			<< "pos:" << vertex.pos
			<< "normal:" << vertex.normal
			<< "}";
}