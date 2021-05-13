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
