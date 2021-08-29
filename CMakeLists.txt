cmake_minimum_required(VERSION 3.10)
project(Sokar)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_SOURCE_DIR src)
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM TRUE)

set(APP_TARGET sokar)

#Biblioteka wątków
find_package(Threads)
find_package(Vulkan REQUIRED FATAL_ERROR)

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

#list(APPEND CONAN_PKGS gdcm/3.0.9)
list(APPEND CONAN_PKGS eigen/3.3.9)
list(APPEND CONAN_PKGS cgal/5.2.1)

include(cmake/conan.cmake)
include(cmake/gdcm.cmake)
include(cmake/qt.cmake)
include(cmake/package.cmake)
include(cmake/shaders.cmake)


run_conan()

find_package(Eigen3 REQUIRED)
find_package(CGAL REQUIRED)
add_compile_definitions(CGAL_AVAILABLE=1)


include(src/3d-party/CMakeLists.txt)


#Tłumaczenia
file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/res/lang/*.ts")
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR}/src ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)

#Pliki projetku
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE CPP_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/src/*.ui")
file(GLOB_RECURSE QRC_FILES "${PROJECT_SOURCE_DIR}/src/*.qrc")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})


file(GLOB_RECURSE SHADER_FILES
        "${PROJECT_SOURCE_DIR}/src/*.vert"
        "${PROJECT_SOURCE_DIR}/src/*.frag"
        )

add_executable(${APP_TARGET}
        ${HEADER_FILES} ${CPP_FILES}
        ${UI_HEADERS} ${QM_FILES}
        res/resources.qrc
        ${QRC_FILES})

add_all_shaders(${APP_TARGET})

target_link_libraries(
        ${APP_TARGET}
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

install(TARGETS ${APP_TARGET} RUNTIME DESTINATION ${INSTALL_DIR})

