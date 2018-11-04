cmake_minimum_required(VERSION 3.10)
project(DiBrowser)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(APP_TARGET DiBrowser)


#Biblioteka DICOM
find_package(GDCM REQUIRED)
include_directories(${GDCM_INCLUDE_DIRS})
link_libraries(${GDCM_LIBRARY_DIRS})

#Bibliotek QT
find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})



add_executable(${APP_TARGET} main.cpp win/main_win.cpp win/main_win.ui convert/convert.h convert/convert.cpp sokar/exception.h)



target_link_libraries(
		${APP_TARGET}
		gdcmMSFF
		Qt5::Widgets
		${EXTRA_LIBS}
)