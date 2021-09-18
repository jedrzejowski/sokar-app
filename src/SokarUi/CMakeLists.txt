SET(SRC_SOKAR_UI
        ../SokarUi.hpp
        SegmentationPipelineEditor.hpp
        SegmentationPipelineEditor.cpp
        SegmentationWindow.cpp
        SegmentationWindow.hpp
        SegmentationResultWidget.hpp
        SegmentationResultWidget.cpp
        DicomToolBar.hpp
        DicomToolBar.cpp
        About.hpp
        About.cpp
        )

list(TRANSFORM SRC_SOKAR_UI PREPEND ${PROJECT_SOURCE_DIR}/src/SokarUi/)


