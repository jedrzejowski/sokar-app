
set(SRC_SOKAR_SCENE
        ../SokarScene.hpp
        MousePosition.cpp
        MousePosition.hpp
        Scene.hpp
        Scene.cpp
        HospitalData.hpp
        HospitalData.cpp
        ImageOrientation.hpp
        ImageOrientation.cpp
        Modality.cpp
        Modality.hpp
        PatientData.hpp
        PatientData.cpp
        PixelSpacing.hpp
        PixelSpacing.cpp
        Avatar.cpp
        Avatar.hpp
        DicomScene.cpp
        DicomScene.hpp
        Indicator.hpp
        Indicator.cpp
        SceneParams.hpp
        )

list(TRANSFORM SRC_SOKAR_SCENE PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

