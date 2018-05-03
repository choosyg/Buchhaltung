add_executable( buchhaltung main.cpp ../resources.qrc )
target_link_libraries( buchhaltung
    buchhaltungLib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Svg
    Qt5::PrintSupport
)
set_target_properties(buchhaltung PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_compile_definitions( buchhaltung PRIVATE _WIN32_WINNT=0x0501)
install(TARGETS buchhaltung RUNTIME DESTINATION .)
