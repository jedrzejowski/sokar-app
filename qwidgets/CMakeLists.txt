
set(src_qwidgets
        qwidgets/scenes/dicomscene.h qwidgets/scenes/dicomscene.cpp
        qwidgets/qdicomdataset.h qwidgets/qdicomdataset.cpp
        qwidgets/qdicomgraphics.h qwidgets/qdicomgraphics.cpp)

set(src_qwidgets ${src_qwidgets}
        ${CMAKE_CURRENT_LIST_DIR}/dicomview.h
        ${CMAKE_CURRENT_LIST_DIR}/dicomview.cpp
        ${CMAKE_CURRENT_LIST_DIR}/dicomview.ui
        ${CMAKE_CURRENT_LIST_DIR}/framechooser.h
        ${CMAKE_CURRENT_LIST_DIR}/framechooser.cpp)


set(src ${src} ${src_qwidgets})

include(${CMAKE_CURRENT_LIST_DIR}/scenes/CMakeLists.txt)