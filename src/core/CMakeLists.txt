add_library(tank_core STATIC
        SpyTank.h
        SpyTank.cpp
        VideoReceiver.h
        VideoReceiver.cpp
)

target_link_libraries(tank_core PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Network
)

target_include_directories(tank_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
