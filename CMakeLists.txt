cmake_minimum_required(VERSION 3.10)
project(Sokar)

#set(CMAKE_CXX_COMPILER "clang")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_SOURCE_DIR src)
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM TRUE)

set(APP_TARGET Sokar)


#Biblioteka gdcmDICOM
find_package(GDCM REQUIRED)
include_directories(${GDCM_INCLUDE_DIRS})
link_libraries(${GDCM_LIBRARY_DIRS})

#Bibliotek QT
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})


include_directories(${PROJECT_SOURCE_DIR}/src)

#Tłumaczenia
file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/res/lang/*.ts")
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR}/src ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)


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
        gdcmMSFF
        Qt5::Widgets
        ${EXTRA_LIBS}
)

#Inne zasoby

file(COPY res/values/ DESTINATION res/values)