#!/usr/bin/env python3

"""Find the maximum benchmark size the project can build."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SCRIPT = ROOT / "scripts" / "generate_benchmark.py"
LEYO = ROOT / "bin" / "leyo.exe"
BENCHMARK = ROOT / "tests" / "benchmark.leyo"


def run(command: list[str]) -> int:
    return subprocess.run(command, cwd=ROOT).returncode


def generate(count: int) -> bool:
    return run([sys.executable, str(SCRIPT), str(count)]) == 0


def build_benchmark() -> int:
    return run([str(LEYO), "build", str(BENCHMARK)])


def main() -> int:
    low = 1
    high = 1

    print("Finding upper limit...")

    while True:
        print()
        print(f"Testing {high}")

        if not generate(high):
            print("Generator failed")
            print()
            print("==========================")
            print(f"Maximum working COUNT: {low}")
            print("==========================")
            return 1

        print("Building...")
        result = build_benchmark()
        print(f"Result: {result}")

        if result == 0:
            low = high
            high *= 2
            continue
        break

    print()
    print(f"Failed at {high}")
    print(f"Searching between {low} and {high}...")

    while low < high:
        mid = (low + high) // 2

        print()
        print(f"Testing {mid}")

        if not generate(mid):
            print("Generator failed")
            print()
            print("==========================")
            print(f"Maximum working COUNT: {low}")
            print("==========================")
            return 1

        result = build_benchmark()
        print(f"Result: {result}")

        if result == 0:
            low = mid
        else:
            high = mid - 1

    print()
    print("==========================")
    print(f"Maximum working COUNT: {low}")
    print("==========================")
    return 0


if __name__ == "__main__":
    sys.exit(main())
