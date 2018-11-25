

set(src_scenes ${src_scenes}
        ${CMAKE_CURRENT_LIST_DIR}/_classdef.h
        ${CMAKE_CURRENT_LIST_DIR}/params.h
        ${CMAKE_CURRENT_LIST_DIR}/scene.h
        ${CMAKE_CURRENT_LIST_DIR}/scene.cpp
        ${CMAKE_CURRENT_LIST_DIR}/dicomscene.h
        ${CMAKE_CURRENT_LIST_DIR}/dicomscene.cpp
        ${CMAKE_CURRENT_LIST_DIR}/dicomscene.cpp
        ${CMAKE_CURRENT_LIST_DIR}/sceneavatar.h
        ${CMAKE_CURRENT_LIST_DIR}/sceneavatar.cpp
        ${CMAKE_CURRENT_LIST_DIR}/dicomsceneset.h
        ${CMAKE_CURRENT_LIST_DIR}/dicomsceneset.cpp)

set(src_scenes ${src_scenes}
        ${CMAKE_CURRENT_LIST_DIR}/indicators/_base.h
        ${CMAKE_CURRENT_LIST_DIR}/indicators/_base.cpp
        ${CMAKE_CURRENT_LIST_DIR}/indicators/pixel.h
        ${CMAKE_CURRENT_LIST_DIR}/indicators/pixel.cpp
        ${CMAKE_CURRENT_LIST_DIR}/indicators/image.h
        ${CMAKE_CURRENT_LIST_DIR}/indicators/image.cpp
        ${CMAKE_CURRENT_LIST_DIR}/indicators/imgorientation.h
        ${CMAKE_CURRENT_LIST_DIR}/indicators/imgorientation.cpp)

set(src_scenes ${src_scenes}
        ${CMAKE_CURRENT_LIST_DIR}/unsupported/unsupported.h
        ${CMAKE_CURRENT_LIST_DIR}/unsupported/unsupported.cpp)

set(src_scenes ${src_scenes}
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/monochrome2.h
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/monochrome2.cpp
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/_window.h
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/windowchooser.h
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/windowchooser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/windowint.h
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/windowint.cpp
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/windowintstatic.h
        ${CMAKE_CURRENT_LIST_DIR}/monochrome2/windowing/windowintdynamic.h)

set(src ${src} ${src_scenes})