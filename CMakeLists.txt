cmake_minimum_required(VERSION 3.16)
project(TeleInfoPlugin VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network Widgets)

qt_standard_project_setup()

qt_add_plugin(TeleInfoPlugin)
target_sources(TeleInfoPlugin PRIVATE
    TeleInfo.ui
    TeleInfoPlugin.cpp TeleInfoPlugin.h
)
target_link_libraries(TeleInfoPlugin PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::Widgets
)


# Resources:
set(TeleInfo_resource_files
    "images/connected.png"
    "images/connecting.png"
    "images/disconnected.png"
    "images/disconnecting.png"
    "images/pause.png"
    "images/play.png"
)

qt_add_resources(TeleInfoPlugin "TeleInfo"
    PREFIX
        "/"
    FILES
        ${TeleInfo_resource_files}
)

install(TARGETS TeleInfoPlugin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
