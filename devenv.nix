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
    DEVENV_OPENCV_FLAGS =
      "-L ${pkgs.opencv3}/lib -lopencv_core -lopencv_imgproc -lopencv_highgui";
    DEVENV_FLAGS =
      " -I ${pkgs.opencv3}/include -I ${pkgs.eigen}/include/eigen3/unsupported -I ${pkgs.eigen}/include/eigen3/Eigen -I ${pkgs.nlohmann_json}/include $(cat ${pkgs.stdenv.cc}/nix-support/cc-cflags) $(cat ${pkgs.stdenv.cc}/nix-support/libcxx-cxxflags) $(cat ${pkgs.stdenv.cc}/nix-support/libc-cflags)";
  };
  # https://devenv.sh/packages/
  packages = [
    pkgs.nixfmt
    pkgs.libsndfile
    pkgs.makeWrapper
    pkgs.pkg-config
    pkgs.which
    pkgs.jq
    pkgs.nlohmann_json
    pkgs.eigen
    pkgs.opencv3
  ];

  # https://devenv.sh/scripts/
  scripts.hello.exec = "echo hello from $GREET";

  scripts.test.exec =
    "DYLD_LIBRARY_PATH=/Users/timpierson/arity/am-vcv/sdk/Rack-SDK:$DYLD_LIBRARY_PATH make clean && make test";

  enterShell = ''
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
      poetry.enable = true;
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
    nixfmt.enable = true;
    deadnix = {
      enable = true;
      excludes = [ "flake.nix" ];
    };
    nil.enable = true;
    statix = {
      enable = true;
      excludes = [ "flake.nix" ];
    };
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
