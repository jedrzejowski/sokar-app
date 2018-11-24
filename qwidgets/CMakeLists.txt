
set(src_qwidgets ${src_qwidgets}
        ${CMAKE_CURRENT_LIST_DIR}/dicomview.h
        ${CMAKE_CURRENT_LIST_DIR}/dicomview.cpp
        ${CMAKE_CURRENT_LIST_DIR}/dicomview.ui

        ${CMAKE_CURRENT_LIST_DIR}/dicomtoolbar.h
        ${CMAKE_CURRENT_LIST_DIR}/dicomtoolbar.cpp

        ${CMAKE_CURRENT_LIST_DIR}/framechooser.h
        ${CMAKE_CURRENT_LIST_DIR}/framechooser.cpp

        ${CMAKE_CURRENT_LIST_DIR}/qdicomdataset.h
        ${CMAKE_CURRENT_LIST_DIR}/qdicomdataset.cpp

        ${CMAKE_CURRENT_LIST_DIR}/dicomgraphics.h
        ${CMAKE_CURRENT_LIST_DIR}/dicomgraphics.cpp)


set(src ${src} ${src_qwidgets})

include(${CMAKE_CURRENT_LIST_DIR}/scenes/CMakeLists.txt)