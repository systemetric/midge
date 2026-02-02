# midge

A helper tool for sending/receiving MQTT payloads over stdin/stdout

## Building

Requirements:

- CMake
- Ninja or GNU Make
- [eclipse-paho-mqtt-c](https://github.com/eclipse-paho/paho.mqtt.c) library

or use the Nix flake.

Ninja build system is recommended, but not required, GNU Make should be fine.

### Cross-compiling

Cross-compiling works with RoboCon brains if you use static libraries (default
if available). Using shared libraries probably won't work because of version
mismatches. You will also need a static libc, probably musl.

...or use the Nix flake to handle this for you, use `cross-aarch64-linux` for brains.
All cross targets are defined as static here.

## Notes

Use `clang-format` for code formatting

## License

See [LICENSE](./LICENSE)

