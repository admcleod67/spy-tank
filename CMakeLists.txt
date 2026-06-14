cmake_minimum_required(VERSION 3.16)
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
        Test
        Quick
        Multimedia
        REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

