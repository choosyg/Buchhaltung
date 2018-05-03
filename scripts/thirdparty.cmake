
#############################################################
# QT
#############################################################
set( CMAKE_PREFIX_PATH "C:/Qt/5.9.2/msvc2017_64" )
find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
find_package(Qt5Svg REQUIRED)
include_directories(${Qt5Svg_INCLUDE_DIRS})
find_package(Qt5PrintSupport REQUIRED)
include_directories(${Qt5PrintSupport_INCLUDE_DIRS})

get_filename_component(QT_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
install( FILES ${QT_DIR}/Qt5Core.dll
               ${QT_DIR}/Qt5Widgets.dll
               ${QT_DIR}/Qt5Gui.dll DESTINATION . )
install( FILES ${QT_DIR}/../plugins/platforms/qwindows.dll DESTINATION ./platforms )
file( COPY ${QT_DIR}/Qt5Core.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Widgets.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Guid.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Svg.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5PrintSupport.dll DESTINATION ${PROJECT_BINARY_DIR}/app )

file( COPY ${QT_DIR}/Qt5Cored.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Widgetsd.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Guid.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5Svgd.dll DESTINATION ${PROJECT_BINARY_DIR}/app )
file( COPY ${QT_DIR}/Qt5PrintSupportd.dll DESTINATION ${PROJECT_BINARY_DIR}/app )

#############################################################
# GTest
#############################################################
set( GTEST_DIR "${PROJECT_SOURCE_DIR}/../gtest-1.7.0" )
include_directories( ${GTEST_DIR}/include )
link_directories( "${GTEST_DIR}/lib" )
set( GTEST_LIBS
    optimized gtest
    debug gtestD
    optimized gtest_main
    debug gtest_mainD
    )
