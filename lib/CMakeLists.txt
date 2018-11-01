set( UIS
    AccountDialog.ui
    InternalTransferDialog.ui
    MainWindow.ui
    PrintPreviewDialog.ui
    TransferDialog.ui
)
set_source_files_properties(${UIS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library( buchhaltungLib
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
    ${UIS}
    )
target_compile_definitions( buchhaltungLib PRIVATE _WIN32_WINNT=0x0501)

add_executable( buchhaltungTest
    Account.test.cpp
    Model.test.cpp
    TransferShare.test.cpp
    )
target_link_libraries( buchhaltungTest
    buchhaltungLib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    ${GTEST_LIBS}
)
target_compile_definitions( buchhaltungTest PRIVATE _WIN32_WINNT=0x0501)
target_compile_definitions (buchhaltungTest PRIVATE TEST_DATA="${CMAKE_SOURCE_DIR}/testdata")
add_test(NAME buchhaltungTest COMMAND ${CMAKE_BINARY_DIR}/app/buchhaltungTest )

