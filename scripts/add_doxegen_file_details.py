from pathlib import Path


COPYRIGHT_HEADER = """/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */"""


def add_file_documentation(root: Path):
    directories = [
        root / "src",
        root / "include",
    ]

    files = []

    for directory in directories:
        if not directory.is_dir():
            print(f"Skipping missing directory: {directory}")
            continue

        files.extend(
            path
            for path in directory.rglob("*")
            if path.is_file() and path.suffix in {".c", ".h"}
        )

    files.sort()

    if not files:
        print("No .c or .h files found in src/ or include/.")
        return

    print(f"Found {len(files)} source files in src/ and include/.")

    for path in files:
        relative_path = path.relative_to(root)

        try:
            content = path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            print(f"Skipping {relative_path}: not valid UTF-8")
            continue

        # Skip files that already have Doxygen @file documentation.
        if "@file" in content[:1500]:
            print(
                f"Skipping {relative_path}: "
                "already has @file documentation"
            )
            continue

        # Make sure the expected copyright header exists.
        if not content.startswith(COPYRIGHT_HEADER):
            print(
                f"Skipping {relative_path}: "
                "copyright header not found at beginning of file"
            )
            continue

        print(f"\nFile: {relative_path}")

        brief = input("What should the @brief say? ").strip()

        if not brief:
            print(f"Skipping {relative_path}: no @brief provided")
            continue

        # Everything after the copyright header.
        remainder = content[len(COPYRIGHT_HEADER):].lstrip("\r\n")

        doxy_header = (
            f"/// @file {path.name}\n"
            f"/// @brief {brief}\n"
            "\n"
        )

        new_content = (
            COPYRIGHT_HEADER
            + "\n\n"
            + doxy_header
            + remainder
        )

        path.write_text(
            new_content,
            encoding="utf-8"
        )

        print(f"Updated {relative_path}")


if __name__ == "__main__":
    project_root = Path(".")

    print("Doxygen @file Documentation Generator")
    print("-------------------------------------")
    print(f"Project root: {project_root.resolve()}")
    print("Only scanning: src/ and include/")
    print("Documentation will be added after the copyright header.\n")

    add_file_documentation(project_root)

    print("\nDone.")