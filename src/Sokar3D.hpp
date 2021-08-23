#pragma once

#include <QtGlobal>
#include <QVulkanInstance>
#include <QVulkanWindowRenderer>
#include <QVector3D>
#include <QVector2D>
#include <SokarMacro.hpp>

#include "SokarGlm.hpp"


namespace Sokar3D {

//	typedef glm::vec3 vec3;
//	typedef glm::vec2 vec2;

    struct MeshVertex;

    class Camera;

    class GameCamera;

    class CenterCamera;

    struct SimpleLight;

    struct SolidMaterial;

    declareSharedClass(Mesh)

    declareSharedClass(StaticMesh)

    declareSharedClass(IndexedMesh)

    class VulkanRenderer;

    class VulkanShader;

    class VulkanWidget;

    class PipelineWrapper;

    class MeshPipeline;

    QVulkanInstance *getVulkanInstance();

    static inline VkDeviceSize makeBuffSizeAligned(VkDeviceSize v, VkDeviceSize byteAlign) {

        return (v + byteAlign - 1) & ~(byteAlign - 1);
    }
}