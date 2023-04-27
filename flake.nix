{
  description = "adaptive volume control";

  inputs = {
      nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = inputs:
    let
      system = "x86_64-linux";
      pkgs = import inputs.nixpkgs {
          inherit system;
      };
      llvm = pkgs.llvmPackages_15;
      nativeBuildInputs = with pkgs; [
        cmake
        gcc
        gnumake
        makeWrapper
      ];
      cmakePackages = with pkgs; [
        portaudio
        alsa-lib
        alsa-plugins
        libpulseaudio
        imgui
      ];
      buildTools = with pkgs; [
        clang-tools
        llvm.clang
        llvm.libcxx
        boost
      ] ++ nativeBuildInputs;
      buildInputs = buildTools ++ cmakePackages;
    in
    {
      defaultPackage.${system} = pkgs.stdenv.mkDerivation {
        name = "adaptive volume control";
        inherit buildInputs;
        inherit nativeBuildInputs;
        postInstall = pkgs.lib.optionalString pkgs.stdenv.isLinux ''
          # wrap binary so alsa can find its plugins
          mv $out/bin/adaptive-volume-control $out/bin/_adaptive-volume-control
          echo "#!/bin/sh" > $out/bin/adaptive-volume-control
          echo "ALSA_PLUGIN_DIR=${pkgs.alsa-plugins}/lib/alsa-lib $out/bin/_adaptive-volume-control" >> $out/bin/adaptive-volume-control
          chmod a+x $out/bin/adaptive-volume-control

          # copy over compile commands for clangd
          cp compile_commands.json $out/
        '';
        src = ./.;
      };

      devShell.${system} = pkgs.mkShell {
        buildInputs = buildInputs ++ [
          pkgs.pkg-config
        ];
      };
    };
}
