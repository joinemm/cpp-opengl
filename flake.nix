{
  description = "cpp-opengl";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    nixpkgs,
    flake-utils,
    ...
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        name = "cpp-opengl";
        pkgs = nixpkgs.legacyPackages.${system};
        buildInputs = with pkgs; [
          glfw
          glew
          stb
          glm
        ];
      in {
        packages = {
          default = pkgs.stdenv.mkDerivation {
            src = ./.;
            makeFlags = ["PREFIX=$(out)"];
            inherit name buildInputs;
          };
        };
        devShells.default = pkgs.mkShell {
          inherit name buildInputs;
        };
      }
    );
}
