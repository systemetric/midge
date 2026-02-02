{
  description = "midge MQTT helper for RoboCon";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      utils,
    }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
      ];
    in
    utils.lib.eachSystem systems (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        packages = {
          default = pkgs.callPackage ./nix/package.nix { };
          cross-x86_64-linux = pkgs.pkgsCross.gnu64.pkgsStatic.callPackage ./nix/package.nix { };
          cross-aarch64-linux =
            pkgs.pkgsCross.aarch64-multiplatform.pkgsStatic.callPackage ./nix/package.nix
              { };
        };

        devShell = pkgs.mkShell {
          packages = with pkgs; [
            clang-tools
            cmake
            ninja

            paho-mqtt-c

            # useful extras
            gdb
            valgrind
          ];
        };
      }
    );
}
