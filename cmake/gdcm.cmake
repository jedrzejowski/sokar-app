
set(GDCM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/gdcm-bin/)

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
        socketxx
        )
