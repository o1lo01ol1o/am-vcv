{ pkgs, ... }:

let
  python310Withoverrides = pkgs.python310.override {
    packageOverrides = _: _: { };

  };

in {
  # https://devenv.sh/basics/
  env = {
    GREET = "devenv";
    # TODO: this uses mac-arch64 
    # TODO: nix-shell -p vcv-rack

    RACK_DIR = (builtins.getEnv "PWD") + "/sdk/Rack-SDK";
    DATA_DIR = (builtins.getEnv "PWD") + "/data";
    CC = "/usr/bin/clang";
    CXX = "/usr/bin/clang++";
    SDKROOT = "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk";
    DEVENV_FLAGS = "-isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk -mmacosx-version-min=11.0 -stdlib=libc++";
    DEVENV_SNDFILE_CFLAGS = "-I/opt/homebrew/include";
    DEVENV_SNDFILE_LIBS = "-L/opt/homebrew/lib -lsndfile";
    DEVENV_OPENCV_FLAGS = "-I ${pkgs.opencv4}/include";
    PKG_CONFIG_PATH = "";
    NIX_CFLAGS_COMPILE = "";
    NIX_CXXSTDLIB_COMPILE = "";
    NIX_CFLAGS_LINK = "";
    NIX_CXXSTDLIB_LINK = "";
    NIX_LDFLAGS = "";
    NIX_LDFLAGS_FOR_TARGET = "";
    NIX_ENFORCE_NO_NATIVE = "";
    LIBRARY_PATH = "";
    DYLD_LIBRARY_PATH = "";
    C_INCLUDE_PATH = "";
    CPLUS_INCLUDE_PATH = "";
    CPATH = "";
  };
  # https://devenv.sh/packages/
  packages = [
    pkgs.codex
    pkgs.jq
    pkgs.nlohmann_json
    pkgs.eigen
    pkgs.opencv4
    pkgs.faust
  ];

  # https://devenv.sh/scripts/
  scripts.hello.exec = "echo hello from $GREET";

  scripts.test.exec =
    "DYLD_LIBRARY_PATH=/Users/timpierson/arity/am-vcv/sdk/Rack-SDK:$DYLD_LIBRARY_PATH make clean && make test";

  scripts.build-faust.exec = ''
    set -euo pipefail

    dsp="examples/pulsar_voice.dsp"
    if [ "$#" -gt 0 ]; then
      dsp="$1"
      shift
    fi

    scripts/generate_faust_vcv.py --force "$dsp" "$@"

    # Prevent passthrough of generator arguments to the builder stage
    set --

    plugin_dir=$(cd "$(dirname "$dsp")" && pwd)/$(basename "$dsp" .dsp)
    scripts/build_vcv_component.py "$plugin_dir"
  '';

  enterShell = ''
    export RACK_DIR="$PWD/sdk/Rack-SDK"
    export DATA_DIR="$PWD/data"
    hello
    git --version
  '';

  languages = {
    nix.enable = true;
    cplusplus.enable = true;
    rust.enable = true;
    c.enable = true;
    #     idris.enable = true;
    python = {
      enable = true;
      uv.enable = true;
      package = pkgs.lib.mkForce python310Withoverrides;
    };
  };

  pre-commit.hooks = {
    # lint shell scripts
    shellcheck.enable = true;
    # format Python code
    # black.enable = true;
    # flake8.enable = true;
    # lint nix
    # nixfmt.enable = true;
    # deadnix = {
    #   enable = true;
    #   excludes = [ "flake.nix" ];
    # };
    # nil.enable = true;
    # statix = {
    #   enable = true;
    #   excludes = [ "flake.nix" ];
    # };
    # If you were using haskell:e
    # # format haskell
    # ormolu.enable = true;
    # cabal-fmt.enable = true;ex
    # # lint haskell 
    # hlint.enable = true;
    clang-format.enable = true;
  };

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";
  # processes.jupyterlab.exec =
  #   "jupyter lab --notebook-dir=./src/test-py/notebooks";
  # See full reference at https://devenv.sh/reference/options/

  # https://devenv.sh/languages/
  #     nix.enable = true;

  # https://devenv.sh/pre-commit-hooks/
  # pre-commit.hooks.shellcheck.enable = true;

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";
  processes.jupyterlab.exec = "jupyter lab --notebook-dir=./notebooks";

  # See full reference at https://devenv.sh/reference/options/
}
