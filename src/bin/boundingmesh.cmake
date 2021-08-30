

add_executable(boundingmesh
        ${CMAKE_CURRENT_LIST_DIR}/boundingmesh.cpp
        ${SRC_3DPRATY_BOUNDINGMESH}
        ${SRC_3DPRATY_BOUNDINGMESH}
        )

target_link_libraries(
        boundingmesh
        LINK_PUBLIC
        Eigen3::Eigen
        CGAL::CGAL
)