cmake_minimum_required(VERSION 3.21)
project(mqtt_shim VERSION 0.1 LANGUAGES C)

find_package(eclipse-paho-mqtt-c REQUIRED)

if(TARGET eclipse-paho-mqtt-c::paho-mqtt3a-static AND TARGET eclipse-paho-mqtt-c::paho-mqtt3c-static)
    message(STATUS "Using static eclipse-paho-mqtt-c")
    set(PAHO_MQTT_TARGET eclipse-paho-mqtt-c::paho-mqtt3a-static eclipse-paho-mqtt-c::paho-mqtt3c-static)
else()
    message(STATUS "Using shared eclipse-paho-mqtt-c")
    set(PAHO_MQTT_TARGET eclipse-paho-mqtt-c::paho-mqtt3a eclipse-paho-mqtt-c::paho-mqtt3c)
endif()

add_subdirectory(src)

