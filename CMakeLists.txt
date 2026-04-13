cmake_minimum_required(VERSION 4.2)
project(spy_tank)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(spy_tank
        main.cpp
        SpyTank.h
        SpyTank.cpp
        MainWindow.h
        MainWindow.cpp
        VideoReceiver.h
        VideoReceiver.cpp
)
target_link_libraries(spy_tank
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

