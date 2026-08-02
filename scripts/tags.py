#!/usr/bin/env python3

"""Create and push a git tag."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    if len(sys.argv) != 2:
        print("Usage: tags.py <tag>", file=sys.stderr)
        return 1

    tag = sys.argv[1]
    if subprocess.run(["git", "tag", tag], cwd=ROOT).returncode != 0:
        return 1
    return subprocess.run(["git", "push", "origin", tag], cwd=ROOT).returncode


if __name__ == "__main__":
    sys.exit(main())
