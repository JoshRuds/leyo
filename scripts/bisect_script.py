#!/usr/bin/env python3

"""Build the recursion test target for `git bisect run`."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def run(command: list[str]) -> int:
    result = subprocess.run(
        command,
        cwd=ROOT,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return result.returncode


def main() -> int:
    if run(["make", "clean"]) != 0:
        return 1
    if run(["make"]) != 0:
        return 1
    if run([str(ROOT / "bin" / "leyo.exe"), "build", "tests/recursion.leyo"]) != 0:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
