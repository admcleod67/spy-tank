find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

qt_add_executable(spy_tank_mobile
    main.cpp
    QmlVideoProducer.h
)

qt_add_qml_module(spy_tank_mobile
    URI com.spytank
    VERSION 1.0
    QML_FILES Main.qml
)

target_link_libraries(spy_tank_mobile PRIVATE
    Qt::Quick
    Qt::Multimedia
    tank_core
)

if(ANDROID)
    set_property(TARGET spy_tank_mobile PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()
