cmake_minimum_required(VERSION 3.10)
project(Sokar)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(APP_TARGET Sokar)


#Biblioteka DICOM
find_package(GDCM REQUIRED)
include_directories(${GDCM_INCLUDE_DIRS})
link_libraries(${GDCM_LIBRARY_DIRS})

#Bibliotek QT
find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})


#include_directories(qwidgets)

qt5_wrap_ui(UIS_HDRS win/main_win.ui)

set(src_proto sokar/exception.h
        qwidgets/scenes/params.h
        qwidgets/scenes/params.cpp
        sokar/dicomtags.h
        sokar/pixel.h)

set(src_mainWin win/main_win.cpp win/main_win.h)

set(src_qwidgets
        qwidgets/qdicomview.h qwidgets/qdicomview.cpp qwidgets/qdicomview.ui
        qwidgets/scenes/scene.h qwidgets/scenes/scene.cpp
        qwidgets/qdicomdataset.h qwidgets/qdicomdataset.cpp
        qwidgets/qdicomgraphics.h qwidgets/qdicomgraphics.cpp)

include(qwidgets/scenes/CMakeLists.txt)

add_executable(${APP_TARGET} main.cpp
        ${src_proto}
        ${src_mainWin}
        ${src_qwidgets}
        ${src_scenes}
        ${UIS_HDRS})


target_link_libraries(
        ${APP_TARGET}
        gdcmMSFF
        Qt5::Widgets
        ${EXTRA_LIBS}
)