
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_GENERATOR "RPM")

set(CPACK_PACKAGE_NAME "sokar")
set(CPACK_PACKAGE_CONTACT "adam@jedrzejowski.pl")
set(CPACK_PACKAGE_VENDOR "Adam Jędrzejowski")

set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)