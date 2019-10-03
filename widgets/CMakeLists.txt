set( UIS
    AccountDialog.ui
    InternalTransferDialog.ui
    MainWindow.ui
    PrintPreviewDialog.ui
    TransferDialog.ui
)
set_source_files_properties(${UIS} PROPERTIES HEADER_FILE_ONLY TRUE)

set( SOURCES
    AccountDialog.h
    AccountDialog.cpp
    AccountOverviewTableModel.h
    AccountOverviewTableModel.cpp
    InternalTransferDialog.h
    InternalTransferDialog.cpp
    MainWindow.h
    MainWindow.cpp
    Model.h
    Model.cpp
    PrintPreviewDialog.h
    PrintPreviewDialog.cpp
    TransferDialog.h
    TransferDialog.cpp
    TransferTableModel.h
    TransferTableModel.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library( AccountingWidgets SHARED ${SOURCES} ${UIS} )
target_compile_definitions( AccountingWidgets PUBLIC _WIN32_WINNT=0x0501 )
target_link_libraries( AccountingWidgets PUBLIC
    AccountingCore
    Qt5::Widgets
    Qt5::Gui
    Qt5::PrintSupport
)
install(TARGETS AccountingWidgets RUNTIME DESTINATION .)

add_executable( AccountingWidgetsTest
    Model.test.cpp
)
target_link_libraries( AccountingWidgetsTest
    AccountingWidgets
    ${GTEST_LIBS}
)
# GTest throws tons of C4996 LNK4099 - Disable it
target_compile_options( AccountingWidgetsTest PRIVATE "/wd4996" )
target_link_options( AccountingWidgetsTest PRIVATE "/ignore:4099" )
target_compile_definitions( AccountingWidgetsTest PRIVATE TEST_DATA="${CMAKE_SOURCE_DIR}/testdata" )
add_test( NAME AccountingWidgetsTest COMMAND ${CMAKE_BINARY_DIR}/app/buchhaltungTest )

