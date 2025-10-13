#!/usr/bin/env python3
"""Build and optionally install a VCV Rack plugin from a generated source tree."""
from __future__ import annotations

import argparse
import contextlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


def _prune_path_var(env: dict[str, str], key: str) -> None:
    value = env.get(key)
    if not value:
        return
    entries = [p for p in value.split(":") if p]
    filtered = [p for p in entries if "/nix/" not in p]
    if filtered:
        env[key] = ":".join(filtered)
    else:
        env.pop(key, None)


def detect_sndfile_flags() -> tuple[str | None, str | None]:
    """Best-effort detection of libsndfile include/lib flags on macOS."""

    def candidate_paths(root: Path) -> list[Path]:
        paths = [root]
        opt_candidate = root / "opt" / "libsndfile"
        if opt_candidate.exists():
            paths.insert(0, opt_candidate)
        return paths

    hinted = os.environ.get("FAUST_SNDFILE_PREFIX")
    search_roots: list[Path] = []
    if hinted:
        search_roots.append(Path(hinted))

    search_roots.extend(
        [
            Path("/opt/homebrew"),
            Path("/usr/local"),
            Path("/usr"),
        ]
    )

    for root in search_roots:
        for candidate in candidate_paths(root):
            include_dir = candidate / "include"
            lib_dir = candidate / "lib"
            if not lib_dir.exists():
                continue
            lib_files = list(lib_dir.glob("libsndfile.*"))
            if not lib_files:
                continue
            cflags = f"-I{include_dir}" if include_dir.exists() else ""
            libs = f"-L{lib_dir} -lsndfile"
            return cflags, libs

    return None, None

VERBOSE = False


class UserError(RuntimeError):
    """Raised for recoverable user-facing errors."""


def run(cmd: list[str], *, cwd: Path | None = None, env: dict[str, str]) -> None:
    """Run a subprocess, streaming stdout/stderr, raising on failure."""
    if VERBOSE:
        location = f" (cwd={cwd})" if cwd else ""
        print(" ".join(cmd) + location)
    subprocess.run(cmd, cwd=cwd, env=env, check=True)


def find_artifact(plugin_dir: Path) -> Path:
    dist_dir = plugin_dir / "dist"
    if not dist_dir.exists():
        raise UserError(f"Expected dist directory missing: {dist_dir}")

    artifacts = sorted(dist_dir.glob("*.vcvplugin"))
    if not artifacts:
        raise UserError(f"No .vcvplugin artifact produced in {dist_dir}")

    return artifacts[0]


def detect_plugin_slug(plugin_dir: Path) -> str:
    manifest = plugin_dir / "plugin.json"
    if manifest.exists():
        try:
            with manifest.open("r", encoding="utf-8") as fh:
                data = json.load(fh)
            slug = data.get("slug")
            if slug:
                return slug
        except Exception:
            pass
    return plugin_dir.name


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "target",
        type=Path,
        help="Path to the generated plugin folder or the source .dsp (stem will be used)",
    )
    parser.add_argument(
        "--out-dir",
        type=Path,
        default=Path("build/faust"),
        help="Directory where the .vcvplugin artifact will be copied",
    )
    parser.add_argument(
        "--install",
        action="store_true",
        default=True,
        help="Install the built .vcvplugin into the Rack user plugin directory (default)",
    )
    parser.add_argument(
        "--no-install",
        action="store_false",
        dest="install",
        help="Skip installing into the Rack plugin directory",
    )
    parser.add_argument(
        "--clean",
        action="store_true",
        help="Run `make clean` before building",
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

    target = args.target.resolve()
    if target.suffix == ".dsp":
        plugin_dir = target.parent / target.stem
    else:
        plugin_dir = target

    if not plugin_dir.exists():
        raise UserError(f"Generated plugin folder not found: {plugin_dir}")
    if not (plugin_dir / "Makefile").exists():
        raise UserError(f"No Makefile found in {plugin_dir}; run generate_faust_vcv.py first")

    env = os.environ.copy()
    repo_root = Path(__file__).resolve().parents[1]
    scripts_bin = repo_root / "scripts"

    original_path = env.get("PATH", "")
    jq_path = shutil.which("jq", path=original_path)
    env["PATH"] = ":".join(
        p for p in original_path.split(":") if p and not p.startswith("/nix/")
    )
    if jq_path:
        jq_dir = str(Path(jq_path).parent)
        if jq_dir not in env["PATH"].split(":"):
            env["PATH"] = f"{env['PATH']}:{jq_dir}" if env["PATH"] else jq_dir

    # Ensure our local scripts (e.g., jq shim) are visible even if PATH was pruned
    path_entries = [p for p in env.get("PATH", "").split(":") if p]
    if str(scripts_bin) not in path_entries:
        path_entries.append(str(scripts_bin))
        env["PATH"] = ":".join(path_entries)

    env["CC"] = "/usr/bin/clang"
    env["CXX"] = "/usr/bin/clang++"
    env["DEVENV_FLAGS"] = (
        os.environ.get("FAUST_DEVENV_FLAGS")
        or os.environ.get("DEVENV_FLAGS")
        or ""
    )
    env["DEVENV_SNDFILE_CFLAGS"] = (
        os.environ.get("FAUST_SNDFILE_CFLAGS")
        or os.environ.get("DEVENV_SNDFILE_CFLAGS")
        or ""
    )
    env["DEVENV_SNDFILE_LIBS"] = (
        os.environ.get("FAUST_SNDFILE_LIBS")
        or os.environ.get("DEVENV_SNDFILE_LIBS")
        or "-lsndfile"
    )

    if env["DEVENV_SNDFILE_LIBS"].strip() == "-lsndfile" or not env["DEVENV_SNDFILE_LIBS"].strip():
        detected_cflags, detected_libs = detect_sndfile_flags()
        if detected_libs is None:
            raise UserError(
                "Could not locate libsndfile. Install it (e.g. via Homebrew) or set "
                "FAUST_SNDFILE_LIBS / FAUST_SNDFILE_CFLAGS."
            )
        env["DEVENV_SNDFILE_LIBS"] = detected_libs
        if not env["DEVENV_SNDFILE_CFLAGS"]:
            env["DEVENV_SNDFILE_CFLAGS"] = detected_cflags or ""

    for nix_var in [
        "NIX_CFLAGS_COMPILE",
        "NIX_CXXSTDLIB_COMPILE",
        "NIX_CFLAGS_LINK",
        "NIX_CXXSTDLIB_LINK",
        "NIX_LDFLAGS",
        "NIX_LDFLAGS_FOR_TARGET",
    ]:
        env.pop(nix_var, None)

    for path_var in [
        "LIBRARY_PATH",
        "LD_LIBRARY_PATH",
        "DYLD_LIBRARY_PATH",
        "CPLUS_INCLUDE_PATH",
        "C_INCLUDE_PATH",
        "CPATH",
    ]:
        _prune_path_var(env, path_var)
    default_rack_dir = repo_root / "sdk" / "Rack-SDK"
    env.setdefault("RACK_DIR", str(default_rack_dir))

    rack_dir = Path(env["RACK_DIR"]).expanduser()
    if not (rack_dir / "plugin.mk").exists():
        if (default_rack_dir / "plugin.mk").exists():
            rack_dir = default_rack_dir
            env["RACK_DIR"] = str(default_rack_dir)
        else:
            raise UserError("RACK_DIR does not look like a Rack SDK")

    plugin_slug = detect_plugin_slug(plugin_dir)
    env.setdefault("SLUG", plugin_slug)

    if args.clean:
        run(["make", "clean"], cwd=plugin_dir, env=env)

    run(["make", "dist"], cwd=plugin_dir, env=env)

    artifact = find_artifact(plugin_dir)

    out_dir = args.out_dir.resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    output_path = out_dir / artifact.name
    shutil.copy2(artifact, output_path)
    print(f"Created {output_path}")

    if args.install:
        if "RACK_PLUGIN_DIR" in os.environ:
            plugins_dir = Path(os.environ["RACK_PLUGIN_DIR"]).expanduser()
        elif "RACK_USER_DIR" in os.environ:
            plugins_dir = Path(os.environ["RACK_USER_DIR"]).expanduser() / "plugins"
        else:
            fallback = Path.home() / "Library" / "Application Support" / "Rack2" / "plugins-mac-arm64"
            plugins_dir = fallback if fallback.exists() else (Path.home() / "Documents" / "Rack2" / "plugins")

        plugins_dir.mkdir(parents=True, exist_ok=True)
        dest_dir = plugins_dir / plugin_slug
        if dest_dir.exists():
            shutil.rmtree(dest_dir)

        with tempfile.NamedTemporaryFile(suffix=".tar", delete=False) as tmp:
            temp_tar = Path(tmp.name)
        try:
            run(["zstd", "-d", "-f", "-o", str(temp_tar), str(output_path)], cwd=None, env=env)
            run(["tar", "-xf", str(temp_tar), "-C", str(plugins_dir)], cwd=None, env=env)
        finally:
            with contextlib.suppress(FileNotFoundError):
                temp_tar.unlink()

        print(f"Installed {plugin_slug} into {plugins_dir}")

    return 0


if __name__ == "__main__":
    try:
        sys.exit(main(sys.argv[1:]))
    except UserError as exc:
        print(f"error: {exc}", file=sys.stderr)
        sys.exit(2)
    except subprocess.CalledProcessError as exc:
        sys.exit(exc.returncode)
