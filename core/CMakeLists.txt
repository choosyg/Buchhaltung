set( SOURCES
    Account.h
    Account.cpp
    Transfer.h
    Transfer.cpp
    TransferShare.h
    TransferShare.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library( AccountingCore SHARED ${SOURCES} )
target_compile_definitions( AccountingCore PUBLIC _WIN32_WINNT=0x0501 )
target_link_libraries( AccountingCore PUBLIC
    Qt5::Core
)
install(TARGETS AccountingCore RUNTIME DESTINATION .)

add_executable( AccountingCoreTest
    Account.test.cpp
    TransferShare.test.cpp
)
target_link_libraries( AccountingCoreTest
    AccountingCore 
    ${GTEST_LIBS}
)
# GTest throws tons of C4996 LNK4099 - Disable it
target_compile_options( AccountingCoreTest PRIVATE "/wd4996" )
target_link_options( AccountingCoreTest PRIVATE "/ignore:4099" )
target_compile_definitions( AccountingCoreTest PRIVATE TEST_DATA="${CMAKE_SOURCE_DIR}/testdata" )
add_test( NAME AccountingCoreTest COMMAND ${CMAKE_BINARY_DIR}/app/buchhaltungTest )

