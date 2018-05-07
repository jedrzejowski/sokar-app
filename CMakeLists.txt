cmake_minimum_required(VERSION 3.10)
project(bucherol)

set(CMAKE_CXX_STANDARD 17)

set(APP_TARGET bucherol)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(${APP_TARGET} main.cpp)


#Biblioteka DICOM
find_package(GDCM REQUIRED)
include_directories(${GDCM_INCLUDE_DIRS})
link_libraries(${GDCM_LIBRARY_DIRS})

#Bibliotek QT
find_package(Qt REQUIRED)
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_BINARY_DIR})

target_link_libraries(
		${APP_TARGET}
		gdcmMSFF
		${QT_LIBRARIES}
		${EXTRA_LIBS}
)