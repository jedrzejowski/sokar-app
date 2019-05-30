cmake_minimum_required(VERSION 3.10)
project(Sokar)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_SOURCE_DIR src)
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM TRUE)

set(APP_TARGET Sokar)

#Biblioteka wątków
find_package(Threads)

#Biblioteka GDCM
set(GDCM_DIR ${CMAKE_CURRENT_LIST_DIR}/../gdcm-bin/)
find_package(GDCM REQUIRED)
include(${GDCM_USE_FILE})
set(GDCM_LIBRARIES
        gdcmCommon
        gdcmDICT
        gdcmDSED
        gdcmIOD
        gdcmexpat
        gdcmzlib
        gdcmcharls
        gdcmopenjp2
        gdcmjpeg12
        gdcmjpeg16
        gdcmjpeg8
        gdcmMEXD
        gdcmMSFF
        socketxx)

#Bibliotek QT
if (WIN32 OR APPLE)

#    if (WIN32)
#        option(QtDir "Qt directory" C:/Qt/5.12.1/msvc2017_64)
#    elseif (APPLE)
#        option(QtDir "Qt directory" /Applications/Qt)
#    endif ()

    set(QtDir "C:/Qt/5.12.1/msvc2017_64" CACHE STRING "Qt directory")

    set(Qt5Widgets_DIR ${QtDir}/lib/cmake/Qt5Widgets/)
    set(Qt5LinguistTools_DIR ${QtDir}/lib/cmake/Qt5LinguistTools/)
endif ()
# Na linuxie powinna być już zainstalowana
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
set(QT_LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)

#Tłumaczenia
file(GLOB TS_FILES "${PROJECT_SOURCE_DIR}/res/lang/*.ts")
qt5_create_translation(QM_FILES ${PROJECT_SOURCE_DIR}/src ${TS_FILES} OPTIONS -source-language en_US -no-obsolete)

#Pliki projetku
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
        LINK_PUBLIC
        ${EXTRA_LIBS}
        ${GDCM_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${QT_LIBRARIES}
)

install(TARGETS ${APP_TARGET} RUNTIME DESTINATION ${INSTALL_DIR})

#Inne zasoby

file(COPY res/values/ DESTINATION res/values)
file(COPY res/img/ DESTINATION res/img)

# Kopiowanie plików bibliotek do folderu instalacji
foreach (lib ${GDCM_LIBRARIES} ${QT_LIBRARIES})

    add_custom_command(TARGET ${APP_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${lib}> $<TARGET_FILE_DIR:${APP_TARGET}>)

endforeach ()
