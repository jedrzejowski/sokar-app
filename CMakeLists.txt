cmake_minimum_required(VERSION 3.10)
project(Sokar)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(APP_TARGET Sokar)


#Biblioteka DICOM
find_package(GDCM REQUIRED)
include_directories(${GDCM_INCLUDE_DIRS})
link_libraries(${GDCM_LIBRARY_DIRS})

#Bibliotek QT
find_package(Qt5Widgets CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})


include_directories(qwidgets)

set(src_proto sokar/exception.h)
set(src_convert convert/convert.h convert/convert.cpp)
set(src_mainWin win/main_win.ui win/main_win.cpp win/main_win.h)
set(src_qwidgets qwidgets/dicomscene.cpp qwidgets/dicomscene.h qwidgets/dicomview.cpp qwidgets/dicomview.h)


add_executable(${APP_TARGET} main.cpp
		${src_proto}
		${src_convert}
		${src_mainWin}
		${src_qwidgets} )


target_link_libraries(
		${APP_TARGET}
		gdcmMSFF
		Qt5::Widgets
		${EXTRA_LIBS}
)