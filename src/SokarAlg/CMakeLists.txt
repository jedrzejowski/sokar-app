
set(SRC_SOKAR_ALG
        ../SokarAlg.hpp
        ../SokarAlg.cpp
        gte/Array3.h
        gte/IntpAkimaUniform3.h
        Algorithm.cpp
        Algorithm.hpp
        Array3.cpp
        Array3.hpp
        BoundingConvexDecomposition.cpp
        BoundingConvexDecomposition.hpp
        CachedVolume.cpp
        CachedVolume.hpp
        DebugVolume.cpp
        DebugVolume.hpp
        DicomVolume.cpp
        DicomVolume.hpp
        BoundingMeshSimplification.cpp
        BoundingMeshSimplification.hpp
        ExampleVolume.cpp
        ExampleVolume.hpp
        EdgeCollapseSimplification.cpp
        EdgeCollapseSimplification.hpp
        GradientVolume.hpp
        GradientVolume.cpp
        InterpolatedVolume.cpp
        InterpolatedVolume.hpp
        IsoRangeDistanceVolumeTransform.hpp
        IsoRangeDistanceVolumeTransform.cpp
        LineInterpolator.hpp
        LineInterpolator.cpp
        MarchingCubes.cpp
        MarchingCubes.hpp
        MarchingTetrahedrons.cpp
        MarchingTetrahedrons.hpp
        MeshSimplificator.cpp
        MeshSimplificator.hpp
        Range.hpp
        RegionGrowthVolume.cpp
        RegionGrowthVolume.hpp
        SegmentationPipeline.cpp
        SegmentationPipeline.hpp
        VertexClustering.cpp
        VertexClustering.hpp
        VertexDecimation.cpp
        VertexDecimation.hpp
        Volume.hpp
        Volume.cpp
        VolumeEnv.cpp
        VolumeEnv.hpp
        VolumeInterpolator.cpp
        VolumeInterpolator.hpp
        VolumeSegmentator.cpp
        VolumeSegmentator.hpp

        bigCubicArray.cpp
        )

list(TRANSFORM SRC_SOKAR_ALG PREPEND ${CMAKE_CURRENT_LIST_DIR}/)