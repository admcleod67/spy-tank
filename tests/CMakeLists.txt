enable_testing()

add_executable(test_spy_tank
        TestSpyTank/TestSpyTank.h
        TestSpyTank/TestSpyTank.cpp
)

target_link_libraries(test_spy_tank PRIVATE
        tank_core
        Qt::Test
)

add_test(NAME test_spy_tank COMMAND test_spy_tank)

add_executable(test_video_receiver
        TestVideoReceiver/TestVideoReceiver.h
        TestVideoReceiver/TestVideoReceiver.cpp
)

target_link_libraries(test_video_receiver PRIVATE
        tank_core
        Qt::Test
)

add_test(NAME test_video_receiver COMMAND test_video_receiver)
