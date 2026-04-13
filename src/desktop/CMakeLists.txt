add_executable(spy_tank
        main.cpp
        MainWindow.h
        MainWindow.cpp
)

target_link_libraries(spy_tank PRIVATE
        tank_core
        Qt::Widgets
)

target_include_directories(spy_tank PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
