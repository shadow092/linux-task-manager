find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Charts)
qt_standard_project_setup()

qt_add_executable(widgit
    main.cpp
)

target_include_directories(widgit PRIVATE headers)

target_link_libraries(widgit PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Charts
)

qt_add_qml_module(widgit
    URI "MyApp"
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        Main.qml
        components/Navbar.qml
        components/Processes.qml
        components/MonitorPage.qml
        components/MonitorChart.qml
    SOURCES
        headers/client.h
        src/client.cpp
)

install(TARGETS widgit DESTINATION /usr/local/bin)