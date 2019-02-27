cmake_minimum_required(VERSION 3.10)
project(Sokar)



set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_SOURCE_DIR src)
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM TRUE)

set(APP_TARGET Sokar)

find_package(Threads)

#Biblioteka GDCM
    set(GDCM_DIR ${CMAKE_CURRENT_LIST_DIR}/lib-win/gdcm/lib/gdcm-2.8/)
if(WIN32)
elseif(APPLE)

elseif(UNIX)
else()
	message( FATAL_ERROR "System not supported" )
endif()

find_package(GDCM REQUIRED)
include_directories(${GDCM_INCLUDE_DIRS})
link_libraries(${GDCM_LIBRARY_DIRS})
include(${GDCM_USE_FILE})

#Bibliotek QT
if(WIN32)
	#C:/Qt/5.12.1/msvc2017_64
    set(Qt5Widgets_DIR C:/Qt/5.12.1/msvc2017_64/lib/cmake/Qt5Widgets/)
    set(Qt5LinguistTools_DIR C:/Qt/5.12.1/msvc2017_64/lib/cmake/Qt5LinguistTools/)
elseif(APPLE)

elseif(UNIX)
else()
	message( FATAL_ERROR "System not supported" )
endif()

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

#Tłumaczenia
file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/res/lang/*.ts")
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR}/src ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE CPP_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE UI_FILES "${PROJECT_SOURCE_DIR}/src/*.ui")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(${APP_TARGET}
        ${HEADER_FILES} ${CPP_FILES}
        ${UI_HEADERS} ${QM_FILES}
        res/resources.qrc)

target_link_libraries(
        ${APP_TARGET}
        ${EXTRA_LIBS}
        gdcmMSFF
        ${CMAKE_THREAD_LIBS_INIT}
        Qt5::Widgets
)

#Inne zasoby

file(COPY res/values/ DESTINATION res/values)
file(COPY res/img/ DESTINATION res/img)