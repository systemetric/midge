add_executable(
    midge
    main.c
)

target_link_libraries(
    midge
    PRIVATE ${PAHO_MQTT_TARGET}
)

install(
    TARGETS midge
    RUNTIME DESTINATION bin
)
