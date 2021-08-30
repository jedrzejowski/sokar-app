
set(SRC_SOKAR_DICOM
        ../SokarDicom.hpp
        DataConventer.cpp
        DataConventer.hpp
        File.cpp
        File.hpp
        )

list(TRANSFORM SRC_SOKAR_DICOM PREPEND ${CMAKE_CURRENT_LIST_DIR}/)