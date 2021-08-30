
set(SRC_SOKAR_3D
        ../Sokar3D.hpp
        ../Sokar3D.cpp
        VulkanWidget.cpp
        Camera.cpp
        PipelineWrapper.cpp
        MeshVertex.cpp
        GameCamera.cpp
        CenterCamera.cpp
        VulkanRenderer.cpp
        MeshPipeline.cpp
        SimpleLight.cpp
        SimpleLight.glsl
        SimpleLight.hpp
        SolidMaterial.cpp
        SolidMaterial.hpp
        MeshPipeline.hpp
        VulkanWidget.hpp
        VulkanRenderer.hpp
        VulkanShader.hpp
        VulkanShader.cpp
        PipelineWrapper.hpp
        MeshVertex.hpp
        Camera.hpp
        CenterCamera.hpp
        GameCamera.hpp
        Mesh.hpp
        Mesh.cpp
        IndexedMesh.hpp
        IndexedMesh.cpp
        CubedIndexedMesh.cpp
        CubedIndexedMesh.hpp
        TriangleListMesh.hpp
        TriangleListMesh.cpp
        )

list(TRANSFORM SRC_SOKAR_3D PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
