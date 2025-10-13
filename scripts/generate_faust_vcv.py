#!/usr/bin/env python3
"""Generate a VCV Rack plugin source tree from a Faust DSP file."""
from __future__ import annotations

import argparse
import os
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

VERBOSE = False


class UserError(RuntimeError):
    """Raised for recoverable user-facing errors."""


def run(cmd: list[str], *, cwd: Path | None = None, env: dict[str, str]) -> None:
    """Run a subprocess, streaming stdout/stderr, raising on failure."""
    if VERBOSE:
        location = f" (cwd={cwd})" if cwd else ""
        print(" ".join(cmd) + location)
    subprocess.run(cmd, cwd=cwd, env=env, check=True)


def ensure_faust_available(env: dict[str, str], faust_root: Path) -> None:
    """Ensure the `faust` binary is available, extending PATH if needed."""
    if shutil.which("faust", path=env.get("PATH")):
        return

    candidate = faust_root / "build" / "bin" / "faust"
    if candidate.exists():
        env["PATH"] = f"{candidate.parent}:{env.get('PATH', '')}"
        return

    raise UserError(
        "Could not find the `faust` executable. Build it with `make -C faust compiler` "
        "or add a faust binary to PATH."
    )


def patch_makefile(makefile: Path) -> None:
    """Inject devenv-friendly flags into the generated Makefile."""
    if not makefile.exists():
        return

    lines = makefile.read_text(encoding="utf-8").splitlines()
    patched: list[str] = []

    for line in lines:
        stripped = line.strip()

        if stripped == "FLAGS +=":
            patched.append(line)
            patched.append("FLAGS += ${DEVENV_FLAGS}")
            continue

        if "pkg-config --cflags sndfile" in line:
            patched.append("CXXFLAGS += ${DEVENV_SNDFILE_CFLAGS}")
            continue

        if "pkg-config --libs sndfile" in line:
            patched.append("LDFLAGS += ${DEVENV_SNDFILE_LIBS}")
            continue

        patched.append(line)

    makefile.write_text("\n".join(patched) + "\n", encoding="utf-8")


def generate_sources(
    dsp_path: Path,
    /,
    *,
    faust_tools_dir: Path,
    env: dict[str, str],
    faust_args: list[str],
    force: bool,
) -> Path:
    """Invoke faust2vcvrack to generate the plugin sources."""
    dsp_name = dsp_path.stem
    plugin_dir = dsp_path.parent / dsp_name

    if plugin_dir.exists():
        if not force:
            raise UserError(
                f"Generated folder {plugin_dir} already exists. Pass --force to overwrite."
            )
        shutil.rmtree(plugin_dir)

    env.setdefault("FAUSTARCH", str(faust_tools_dir.parent.parent / "architecture"))
    env["PATH"] = f"{faust_tools_dir}:{env.get('PATH', '')}"

    cmd = [str(faust_tools_dir / "faust2vcvrack"), "-source", *faust_args, dsp_path.name]
    run(cmd, cwd=dsp_path.parent, env=env)

    patch_makefile(plugin_dir / "Makefile")

    return plugin_dir


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("dsp", type=Path, help="Path to the Faust .dsp source")
    parser.add_argument(
        "--voices",
        type=int,
        default=None,
        help="Number of polyphonic voices (passed to faust2vcvrack -nvoices)",
    )
    parser.add_argument(
        "--soundfile",
        action="store_true",
        help="Enable faust2vcvrack -soundfile handling",
    )
    parser.add_argument(
        "--version",
        type=str,
        default=None,
        help="Plugin major version (faust2vcvrack -version argument)",
    )
    parser.add_argument(
        "--extra-faust",
        metavar="ARG",
        action="append",
        default=[],
        help="Additional arguments forwarded to the faust compiler",
    )
    parser.add_argument(
        "--force",
        action="store_true",
        help="Overwrite an existing generated folder if present",
    )
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Show commands executed during the build",
    )
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)

    global VERBOSE
    VERBOSE = args.verbose

    dsp_path = args.dsp.resolve()

    if dsp_path.suffix != ".dsp":
        raise UserError("Input file must have a .dsp extension")
    if not dsp_path.exists():
        raise UserError(f"DSP file not found: {dsp_path}")

    repo_root = Path(__file__).resolve().parents[1]
    faust_root = repo_root / "faust"
    faust_tools_dir = faust_root / "tools" / "faust2appls"

    if not faust_tools_dir.exists():
        raise UserError(f"Faust tools directory missing: {faust_tools_dir}")

    env = os.environ.copy()
    default_rack_dir = repo_root / "sdk" / "Rack-SDK"
    env.setdefault("RACK_DIR", str(default_rack_dir))

    rack_dir = Path(env["RACK_DIR"]).expanduser()
    if not (rack_dir / "plugin.mk").exists():
        if (default_rack_dir / "plugin.mk").exists():
            rack_dir = default_rack_dir
            env["RACK_DIR"] = str(default_rack_dir)
        else:
            raise UserError(
                "RACK_DIR does not look like a Rack SDK. Set RACK_DIR to the Rack-SDK root."
            )

    ensure_faust_available(env, faust_root)

    faust_args: list[str] = []
    if args.voices:
        if args.voices < 1:
            raise UserError("--voices must be >= 1")
        faust_args += ["-nvoices", str(args.voices)]
    if args.soundfile:
        faust_args.append("-soundfile")
    if args.version:
        faust_args += ["-version", args.version]
    for extra in args.extra_faust:
        faust_args.extend(shlex.split(extra))

    plugin_dir = generate_sources(
        dsp_path,
        faust_tools_dir=faust_tools_dir,
        env=env,
        faust_args=faust_args,
        force=args.force,
    )

    print(f"Generated Rack plugin sources in {plugin_dir}")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main(sys.argv[1:]))
    except UserError as exc:
        print(f"error: {exc}", file=sys.stderr)
        sys.exit(2)
