
add_executable(sokar
        ${CMAKE_CURRENT_LIST_DIR}/sokar.cpp
        ${SRC_SOKAR}
        ${SRC_3DPRATY_BOUNDINGMESH}
        ${SRC_3DPRATY_BOUNDINGMESH}
        ${UI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/res/resources.qrc
        )

message(
       " ${CMAKE_CURRENT_LIST_DIR}/sokar.cpp
        ${SRC_SOKAR}
        ${SRC_3DPRATY_BOUNDINGMESH}
        ${SRC_3DPRATY_BOUNDINGMESH}
        ${UI_HEADERS}
        ${CMAKE_CURRENT_SOURCE_DIR}/res/resources.qrc")

add_all_shaders(sokar)

target_link_libraries(
        sokar
        LINK_PUBLIC
        ${EXTRA_LIBS}
        ${GDCM_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${QT_LIBRARIES}
        ${OpenCV_LIBS}
        glfw
        ${Vulkan_LIBRARY}
        Eigen3::Eigen
        CGAL::CGAL
)

install(TARGETS sokar RUNTIME DESTINATION ${INSTALL_DIR})


