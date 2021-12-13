

add_executable(igl703
        ${CMAKE_CURRENT_LIST_DIR}/igl703.cpp
        )

target_link_libraries(
        igl703
        LINK_PUBLIC
        Eigen3::Eigen
        igl::igl
)