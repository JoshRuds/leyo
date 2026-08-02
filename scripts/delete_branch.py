#!/usr/bin/env python3

"""Interactively delete local and remote git branches."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def git(*args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["git", *args],
        cwd=ROOT,
        text=True,
        capture_output=True,
        check=False,
    )


def prompt(text: str) -> str:
    return input(text).strip()


def main() -> int:
    print("========================================")
    print("      Git Branch Cleanup Utility")
    print("========================================")
    print()

    if git("fetch", "--prune").returncode != 0:
        return 1

    current = git("branch", "--show-current")
    if current.returncode != 0:
        return 1
    current_branch = current.stdout.strip()

    print(f"Current branch: {current_branch}")
    print()
    print("Available branches:")
    print()

    branches: list[str] = []
    branch_list = git("branch")
    if branch_list.returncode != 0:
        return 1

    for line in branch_list.stdout.splitlines():
        branch = re.sub(r"^[* ]*", "", line).strip()
        if not branch or branch == current_branch:
            continue
        branches.append(branch)
        print(f"  {len(branches):2d}) {branch}")

    print()
    if not branches:
        print("No branches available to delete.")
        return 0

    print("Enter branch numbers to delete.")
    print("Examples:")
    print("  1")
    print("  1 3 5")
    print("  all")
    print()

    selection = prompt("> ")
    if selection == "all":
        selected = branches[:]
    else:
        selected = []
        for num in selection.split():
            if num.isdigit():
                idx = int(num)
                if 1 <= idx <= len(branches):
                    selected.append(branches[idx - 1])

    if not selected:
        print()
        print("Nothing selected.")
        return 0

    print()
    print("The following branches will be deleted:")
    print()
    for branch in selected:
        print(f"  - {branch}")

    print()
    confirm = prompt("Continue? (y/N): ")
    if confirm.lower() != "y":
        print("Cancelled.")
        return 0

    print()
    for branch in selected:
        print(f"Deleting local branch: {branch}")
        if git("branch", "-D", branch).returncode != 0:
            return 1

        remote = git("ls-remote", "--heads", "origin", branch)
        if remote.returncode != 0:
            return 1

        if remote.stdout.strip():
            print(f"Deleting remote branch: {branch}")
            if git("push", "origin", "--delete", branch).returncode != 0:
                return 1
        else:
            print(f"No remote branch named '{branch}'.")

        print()

    if git("fetch", "--prune").returncode != 0:
        return 1

    print("========================================")
    print("Done!")
    print("Remaining branches:")
    return git("branch").returncode


if __name__ == "__main__":
    sys.exit(main())
