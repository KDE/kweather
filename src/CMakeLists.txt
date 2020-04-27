set(kweather_SRCS
    main.cpp
    )

qt5_add_resources(RESOURCES resources.qrc)
add_executable(kweather ${kweather_SRCS} ${RESOURCES})
target_link_libraries(kweather Qt5::Core Qt5::Qml Qt5::Quick Qt5::Svg Qt5::Network)
install(TARGETS kweather ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
