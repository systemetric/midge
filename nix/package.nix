{
  stdenv,
  ninja,
  cmake,
  paho-mqtt-c,
}:

stdenv.mkDerivation {
  name = "mqtt_shim";
  version = "0.1";

  src = builtins.filterSource (path: type: type != "directory" || baseNameOf path != "build") ./..;

  nativeBuildInputs = [
    ninja
    cmake
  ];

  buildInputs = [
    paho-mqtt-c
  ];
}
