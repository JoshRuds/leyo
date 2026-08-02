#!/usr/bin/env python3

"""Fetch all remote branches and create matching local tracking branches."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def git(*args: str, capture_output: bool = False) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["git", *args],
        cwd=ROOT,
        text=True,
        capture_output=capture_output,
        check=False,
    )


def main() -> int:
    print("Fetching latest branches...")
    if git("fetch", "--all", "--prune").returncode != 0:
        return 1

    print()

    remote = git("branch", "-r", capture_output=True)
    if remote.returncode != 0:
        return 1

    for line in remote.stdout.splitlines():
        branch = line.strip()
        if not branch.startswith("origin/"):
            continue
        if branch == "origin/HEAD":
            continue

        local = branch.removeprefix("origin/")
        exists = git("show-ref", "--verify", "--quiet", f"refs/heads/{local}")
        if exists.returncode == 0:
            print(f"✓ {local} already exists locally")
            continue

        print(f"Creating local branch: {local}")
        if git("branch", "--track", local, f"origin/{local}").returncode != 0:
            return 1

    print()
    print("Done!")
    print()
    return git("branch").returncode


if __name__ == "__main__":
    sys.exit(main())
