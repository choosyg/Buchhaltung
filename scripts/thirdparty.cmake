
#############################################################
# QT
#############################################################
set( CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.3/5.12.3/msvc2017_64" )
find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
find_package(Qt5Svg REQUIRED)
include_directories(${Qt5Svg_INCLUDE_DIRS})
find_package(Qt5PrintSupport REQUIRED)
include_directories(${Qt5PrintSupport_INCLUDE_DIRS})

get_filename_component(QT_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
set( QT_INSTALL_DLLS
    ${QT_DIR}/Qt5Core.dll
    ${QT_DIR}/Qt5Widgets.dll
    ${QT_DIR}/Qt5Gui.dll
    ${QT_DIR}/Qt5Svg.dll
    ${QT_DIR}/Qt5PrintSupport.dll
)

set( QT_INSTALL_PLUGINS
    ${QT_DIR}/../plugins/platforms/qwindows.dll
)

install( FILES ${QT_INSTALL_DLLS} DESTINATION . )
install( FILES ${QT_INSTALL_PLUGINS} DESTINATION ./platforms )

file( COPY ${QT_INSTALL_DLLS} DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${QT_INSTALL_PLUGINS} DESTINATION ${PROJECT_BINARY_DIR}/platforms )

file( COPY ${QT_DIR}/Qt5Cored.dll DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${QT_DIR}/Qt5Widgetsd.dll DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${QT_DIR}/Qt5Guid.dll DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${QT_DIR}/Qt5Svgd.dll DESTINATION ${PROJECT_BINARY_DIR} )
file( COPY ${QT_DIR}/Qt5PrintSupportd.dll DESTINATION ${PROJECT_BINARY_DIR} )

#############################################################
# GTest
#############################################################
set( GTEST_DIR "${PROJECT_SOURCE_DIR}/../Resources/3rd-party-windows-x86_64/gtest-vc141-1.7.0" )
include_directories( ${GTEST_DIR}/include )
link_directories( "${GTEST_DIR}/lib" )
set( GTEST_LIBS
    optimized gtest
    debug gtestD
    optimized gtest_main
    debug gtest_mainD
    )
