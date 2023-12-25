{ pkgs, ... }:

let 
  python310Withoverrides = pkgs.python310.override {
    packageOverrides = _: _: {
    };

  };

in
{
  # https://devenv.sh/basics/
  env.GREET = "devenv";
  # TODO: this uses mac-arch64 
  
  env.RACK_DIR =(builtins.getEnv "PWD") + "/sdk/Rack-SDK";

  # https://devenv.sh/packages/
  packages = [  pkgs.git
    pkgs.clang
    pkgs.stdenv.cc.cc.lib
    pkgs.cmake
    pkgs.nixfmt
  ];

  # https://devenv.sh/scripts/
  scripts.hello.exec = "echo hello from $GREET";

  enterShell = ''
    hello
    git --version
  '';

  languages.nix.enable = true;
  languages.cplusplus.enable = true;
  languages.c.enable = true;
  languages.idris.enable = true;
  languages.python = {
    enable = true;
    poetry.enable = true;
    package = pkgs.lib.mkForce python310Withoverrides;
  };

  pre-commit.hooks = {
    # lint shell scripts
    shellcheck.enable = true;
    # format Python code
    black.enable = true;
    flake8.enable = true;
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
    # If you were using haskell:
    # # format haskell
    # ormolu.enable = true;
    # cabal-fmt.enable = true;ex
    # # lint haskell 
    # hlint.enable = true;
    clang-format.enable =true;
  };

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";
  processes.jupyterlab.exec =
    "jupyter lab --notebook-dir=./src/test-py/notebooks";
  # See full reference at https://devenv.sh/reference/options/



  # https://devenv.sh/languages/
  # languages.nix.enable = true;

  # https://devenv.sh/pre-commit-hooks/
  # pre-commit.hooks.shellcheck.enable = true;

  # https://devenv.sh/processes/
  # processes.ping.exec = "ping example.com";

  # See full reference at https://devenv.sh/reference/options/
}
