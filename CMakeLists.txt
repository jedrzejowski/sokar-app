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

list(APPEND CONAN_PKGS gdcm/3.0.9)
list(APPEND CONAN_PKGS eigen/3.3.9)
list(APPEND CONAN_PKGS cgal/5.2.1)
list(APPEND CONAN_PKGS libigl/2.3.0)

include(cmake/conan.cmake)
run_conan()

include(cmake/gdcm.cmake)
include(cmake/qt.cmake)
include(cmake/package.cmake)
include(cmake/shaders.cmake)
include(cmake/other.cmake)

include(src/3d-party/CMakeLists.txt)
include(src/Sokar.cmake)

#Tłumaczenia
file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/res/lang/*.ts")
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR}/src ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)

#Pliki projetku
include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/src/*.ui")
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

include(src/bin/boundingmesh-cli.cmake)
include(src/bin/bounding-convex-decomposition-cli.cmake)
include(src/bin/sokar.cmake)
#include(src/bin/igl703.cmake)



