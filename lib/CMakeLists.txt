set( UIS
    AccountDialog.ui
    InternalTransferDialog.ui
    MainWindow.ui
    PrintPreviewDialog.ui
    TransferDialog.ui
)
set_source_files_properties(${UIS} PROPERTIES HEADER_FILE_ONLY TRUE)

set( SOURCES
    Account.h
    Account.cpp
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
    Transfer.h
    Transfer.cpp
    TransferDialog.h
    TransferDialog.cpp
    TransferShare.h
    TransferShare.cpp
    TransferTableModel.h
    TransferTableModel.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library( buchhaltung SHARED ${SOURCES} ${UIS} )
target_compile_definitions( buchhaltung PUBLIC _WIN32_WINNT=0x0501 )
target_link_libraries( buchhaltung PUBLIC
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
)
install(TARGETS buchhaltung RUNTIME DESTINATION .)

add_executable( buchhaltungTest
    Account.test.cpp
    Model.test.cpp
    TransferShare.test.cpp
)
target_link_libraries( buchhaltungTest
    buchhaltung
    ${GTEST_LIBS}
)
# GTest throws tons of C4996 LNK4099 - Disable it
target_compile_options( buchhaltungTest PRIVATE "/wd4996" )
target_link_options( buchhaltungTest PRIVATE "/ignore:4099" )
target_compile_definitions( buchhaltungTest PRIVATE TEST_DATA="${CMAKE_SOURCE_DIR}/testdata" )
add_test( NAME buchhaltungTest COMMAND ${CMAKE_BINARY_DIR}/app/buchhaltungTest )

