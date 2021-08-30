
set(SRC_3DPRATY_BOUNDINGMESH
        ContractionUtils.cpp
        ContractionUtils.h
        Decimator.cpp
        Decimator.h
        FileUtils.cpp
        FileUtils.h
        Mesh.cpp
        Mesh.h
        MetricGenerator.cpp
        MetricGenerator.h
        OptimizerInterface.h
        Primitives.cpp
        Primitives.h
        Segmenter.h
        SegmenterDownsampling.cpp
        SegmenterDownsampling.h
        SegmenterSimple.cpp
        SegmenterSimple.h
        SimpleOptimizer.cpp
        SimpleOptimizer.h
        Split.cpp
        Split.h
        VoxelSet.cpp
        VoxelSet.h
        VoxelSubset.cpp
        VoxelSubset.h
        )

list(TRANSFORM SRC_3DPRATY_BOUNDINGMESH PREPEND ${CMAKE_CURRENT_LIST_DIR}/boundingmesh/)


set(SRC_3DPRATY_OPTIONPARSER
        optionparser.h
        )

list(TRANSFORM SRC_3DPRATY_OPTIONPARSER PREPEND ${CMAKE_CURRENT_LIST_DIR}/optionparser)


include_directories(${CMAKE_CURRENT_LIST_DIR})


include_directories(${CMAKE_CURRENT_LIST_DIR})
