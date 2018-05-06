cmake_minimum_required (VERSION 3.9.0)
project( Buchhaltung )

set( Buchhaltung_VERSION_MAJOR 1 )
set( Buchhaltung_VERSION_MINOR 0 )
set( Buchhaltung_VERSION_PATCH 0 )

include( scripts/thirdparty.cmake )
include(CTest)
enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories("${Buchhaltung_SOURCE_DIR}")

add_subdirectory( lib )
add_subdirectory( app )

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_VENDOR "Schiffler")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It's great")
set(CPACK_PACKAGE_VERSION ${Buchhaltung_VERSION_MAJOR}.${Buchhaltung_VERSION_MINOR}.${Buchhaltung_VERSION_PATCH}-${CMAKE_BUILD_TYPE})
set(CPACK_PACKAGE_VERSION_MAJOR ${Buchhaltung_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Buchhaltung_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Buchhaltung_VERSION_PATCH})
include(CPack)
