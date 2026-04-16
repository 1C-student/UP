cmake_minimum_required(VERSION 3.19)
project(PartyWork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql PrintSupport)

qt_add_executable(PartyWork
    WIN32 MACOSX_BUNDLE
    main.cpp
    database.cpp
    database.h
    authwindow.cpp
    authwindow.h
    authwindow.ui
    adminwindow.cpp
    adminwindow.h
    adminwindow.ui
    userwindow.cpp
    userwindow.h
    userwindow.ui
    editmemberdialog.cpp
    editmemberdialog.h
    editmemberdialog.ui
    registerwindow.cpp
    registerwindow.h
    registerwindow.ui
    aboutwindow.cpp
    aboutwindow.h
    aboutwindow.ui
    helpwindow.cpp
    helpwindow.h
    helpwindow.ui
)

target_link_libraries(PartyWork
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::PrintSupport
)

file(COPY ${CMAKE_SOURCE_DIR}/photos DESTINATION ${CMAKE_BINARY_DIR})

install(TARGETS PartyWork
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
