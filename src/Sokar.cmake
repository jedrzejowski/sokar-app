
SET(SRC_SOKAR
        SokarGlm.cpp
        SokarGlm.hpp
        SokarMacro.hpp
        SokarGdcm.hpp
        SokarException.hpp
        )

list(TRANSFORM SRC_SOKAR PREPEND ${CMAKE_CURRENT_LIST_DIR}/)

include(${CMAKE_CURRENT_LIST_DIR}/Sokar3D/CMakeLists.txt)
list(APPEND SRC_SOKAR ${SRC_SOKAR_3D})

include(${CMAKE_CURRENT_LIST_DIR}/SokarAlg/CMakeLists.txt)
list(APPEND SRC_SOKAR ${SRC_SOKAR_ALG})

include(${CMAKE_CURRENT_LIST_DIR}/SokarDicom/CMakeLists.txt)
list(APPEND SRC_SOKAR ${SRC_SOKAR_DICOM})

include(${CMAKE_CURRENT_LIST_DIR}/SokarLib/CMakeLists.txt)
list(APPEND SRC_SOKAR ${SRC_SOKAR_LIB})

include(${CMAKE_CURRENT_LIST_DIR}/SokarScene/CMakeLists.txt)
list(APPEND SRC_SOKAR ${SRC_SOKAR_SCENE})

include(${CMAKE_CURRENT_LIST_DIR}/SokarUi/CMakeLists.txt)
list(APPEND SRC_SOKAR ${SRC_SOKAR_UI})


set(SRC_SOKAR_LEGACY
        dicomview/framechooser.cpp
        dicomview/moviebar.cpp
        dicomview/dataset.cpp
        dicomview/graphics.cpp
        dicomview/scenes/exception
        dicomview/scenes/exception/exception.h
        dicomview/scenes/exception/exception.cpp
        dicomview/scenes/sets/_sceneset.cpp
        dicomview/scenes/sets/_sceneset.h
        dicomview/scenes/sets/fileset.cpp
        dicomview/scenes/sets/fileset.h
        dicomview/scenes/sets/frameset.cpp
        dicomview/scenes/sets/frameset.h
        dicomview/scenes/sets/sceneseq.cpp
        dicomview/scenes/sets/sceneseq.h
        dicomview/scenes/lumbluered/lumbluered.cpp
        dicomview/scenes/lumbluered/lumbluered.h
        dicomview/scenes/monochrome/windowing/windowint.h
        dicomview/scenes/monochrome/windowing/_window.cpp
        dicomview/scenes/monochrome/windowing/windowint.cpp
        dicomview/scenes/monochrome/windowing/windowint.menu.cpp
        dicomview/scenes/monochrome/windowing/windowintdynamic.h
        dicomview/scenes/monochrome/windowing/windowintstatic.h
        dicomview/scenes/monochrome/windowing/_window.h
        dicomview/scenes/monochrome/monochrome.h
        dicomview/scenes/monochrome/monochrome.cpp
        dicomview/scenes/redgreenblue/redgreenblue.cpp
        dicomview/scenes/redgreenblue/redgreenblue.h
        dicomview/scenes/moviemode.cpp
        dicomview/scenes/indicators
        dicomview/scenes/moviemode.h
        dicomview/dataset.h
        dicomview/graphics.h
        dicomview/dicomview.cpp
        dicomview/dicomview.h
        dicomview/framechooser.h
        dicomview/moviebar.h
        sokar/qt.h
        sokar/gdcmSokar.h
        sokar/palette.h
        sokar/speedtest.h
        sokar/pixel.h
        sokar/exception.h
        sokar/settings.cpp
        sokar/settings.h
        sokar/qt.cpp
        sokar/palette.cpp
        win/mainwindow.cpp
        win/mainwindow.h
        widget/dicomtabs.h
        widget/filetree.cpp
        widget/filetree.h
        widget/dicomtabs.cpp
        _classdef.h
        )

list(TRANSFORM SRC_SOKAR_LEGACY PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
list(APPEND SRC_SOKAR ${SRC_SOKAR_LEGACY})