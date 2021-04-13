#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <QtGlobal>
#include <QVulkanInstance>
#include <QVulkanWindowRenderer>
#include <QVector3D>
#include <QVector2D>


namespace Sokar3D {

	typedef glm::vec3 vec3;
	typedef glm::vec2 vec2;

	struct MeshVertex;

	class Camera;

	class Mesh;

	class Renderer;

	class Shader;

	struct ShaderData;

	class VulkanWidget;

	class TestWindow;

	class PipelineWrapper;

	class MeshPipeline;

	QVulkanInstance *getVulkanInstance();
}