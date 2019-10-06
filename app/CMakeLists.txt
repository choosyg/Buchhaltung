add_executable( BuchhaltungApp main.cpp ../resources.qrc )
target_link_libraries( BuchhaltungApp
    AccountingWidgets
)
set_target_properties( BuchhaltungApp PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup" )
install( TARGETS BuchhaltungApp RUNTIME DESTINATION . )
