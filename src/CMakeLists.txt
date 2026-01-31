add_executable(
    mqtt_shim
    main.c
)

target_link_libraries(
    mqtt_shim
    PRIVATE ${PAHO_MQTT_TARGET}
)

install(
    TARGETS mqtt_shim
    RUNTIME DESTINATION bin
)
