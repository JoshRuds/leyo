#!/usr/bin/env python3

import re
import sys
from pathlib import Path


SRC_DIR = Path("src")
INCLUDE_DIR = Path("include")


# C keywords that introduce control-flow constructs rather than functions.
CONTROL_KEYWORDS = {
    "if",
    "for",
    "while",
    "switch",
    "catch",
}


# Matches the beginning of a C function definition.
#
# Examples:
#   int foo(
#   static void foo(
#   const char *foo(
#   unsigned int foo(
#   struct Foo *foo(
#
# The actual function name is captured separately.
FUNCTION_RE = re.compile(
    r"""
    (?P<static>\bstatic\s+)?

    # Function qualifiers.
    (?:
        inline\s+
        | extern\s+
        | _Noreturn\s+
    )*

    (?P<prefix>
        (?:
            const\s+
        )?

        (?:
            unsigned\s+
            | signed\s+
            | long\s+
            | short\s+
        )*

        (?:
            void
            | char
            | short
            | int
            | long
            | float
            | double
            | size_t
            | [A-Za-z_]\w*
        )

        (?:
            \s*\*
        )*

        \s+
    )

    (?P<name>[A-Za-z_]\w*)
    \s*\(
    """,
    re.VERBOSE,
)


def strip_comments_and_strings(text):
    """
    Remove C comments and replace string/character literals with spaces.

    Newlines are preserved so that line numbers remain correct.
    """
    result = []
    i = 0
    length = len(text)

    in_block_comment = False
    in_string = False
    in_char = False
    escape = False

    while i < length:
        char = text[i]
        next_char = text[i + 1] if i + 1 < length else ""

        # Inside a block comment.
        if in_block_comment:
            if char == "*" and next_char == "/":
                result.extend("  ")
                i += 2
                in_block_comment = False
            else:
                result.append("\n" if char == "\n" else " ")
                i += 1

            continue

        # Inside a string literal.
        if in_string:
            if char == "\n":
                result.append("\n")
                in_string = False
                escape = False
            elif escape:
                result.append(" ")
                escape = False
            elif char == "\\":
                result.append(" ")
                escape = True
            elif char == '"':
                result.append(" ")
                in_string = False
            else:
                result.append(" ")

            i += 1
            continue

        # Inside a character literal.
        if in_char:
            if char == "\n":
                result.append("\n")
                in_char = False
                escape = False
            elif escape:
                result.append(" ")
                escape = False
            elif char == "\\":
                result.append(" ")
                escape = True
            elif char == "'":
                result.append(" ")
                in_char = False
            else:
                result.append(" ")

            i += 1
            continue

        # Start of a block comment.
        if char == "/" and next_char == "*":
            result.extend("  ")
            i += 2
            in_block_comment = True
            continue

        # Start of a line comment.
        if char == "/" and next_char == "/":
            result.extend("  ")
            i += 2

            while i < length and text[i] != "\n":
                result.append(" ")
                i += 1

            continue

        # Start of a string literal.
        if char == '"':
            result.append(" ")
            i += 1
            in_string = True
            escape = False
            continue

        # Start of a character literal.
        if char == "'":
            result.append(" ")
            i += 1
            in_char = True
            escape = False
            continue

        result.append(char)
        i += 1

    return "".join(result)


def has_doxygen_comment(lines, line_index):
    """
    Check whether a /// Doxygen comment directly precedes a declaration.

    Blank lines between the comment and declaration are allowed.
    """

    i = line_index - 1

    # Skip blank lines.
    while i >= 0 and not lines[i].strip():
        i -= 1

    if i < 0:
        return False

    # Require at least one /// line immediately preceding
    # the declaration/comment block.
    found = False

    while i >= 0:
        line = lines[i].strip()

        if line.startswith("///"):
            found = True
            i -= 1
            continue

        break

    return found


def find_header(function_name):
    """
    Find the header containing a declaration for a function.

    Searches all .h files under include/.
    """

    pattern = re.compile(
        rf"\b{re.escape(function_name)}\s*\("
    )

    for header in INCLUDE_DIR.rglob("*.h"):
        try:
            content = header.read_text(
                encoding="utf-8"
            )
        except (UnicodeDecodeError, OSError):
            continue

        # Remove comments/strings before searching.
        cleaned = strip_comments_and_strings(content)

        if pattern.search(cleaned):
            return header

    return None


def header_has_documentation(header, function_name):
    """
    Check whether a function declaration in a header has
    a preceding /// Doxygen comment.
    """

    try:
        content = header.read_text(
            encoding="utf-8"
        )
    except (UnicodeDecodeError, OSError):
        return False

    lines = content.splitlines()
    cleaned = strip_comments_and_strings(content).splitlines()

    pattern = re.compile(
        rf"\b{re.escape(function_name)}\s*\("
    )

    for i, line in enumerate(cleaned):
        if pattern.search(line):
            return has_doxygen_comment(
                lines,
                i,
            )

    return False


def find_matching_paren(text, opening_index):
    """
    Find the closing ')' matching the '(' at opening_index.

    Returns None if no matching parenthesis exists.
    """

    depth = 0

    for i in range(opening_index, len(text)):
        if text[i] == "(":
            depth += 1

        elif text[i] == ")":
            depth -= 1

            if depth == 0:
                return i

    return None


def find_functions(source_path):
    """
    Find function definitions at global scope.

    Returns:
        lines, functions

    Each function contains:
        name
        line
        static
    """

    try:
        original_text = source_path.read_text(
            encoding="utf-8"
        )
    except (UnicodeDecodeError, OSError) as exc:
        print(
            f"Warning: could not read "
            f"{source_path}: {exc}",
            file=sys.stderr,
        )
        return [], []

    lines = original_text.splitlines()

    # Strip comments and literals for parsing while preserving
    # line structure.
    cleaned_text = strip_comments_and_strings(
        original_text
    )

    cleaned_lines = cleaned_text.splitlines()

    functions = []

    brace_depth = 0

    # Text accumulated while looking for a declaration.
    declaration = ""

    # Original source line where the declaration began.
    declaration_start = None

    # Track whether we are currently inside a preprocessor
    # conditional/directive.
    preprocessor_continuation = False

    for i, line in enumerate(cleaned_lines):

        original_line = lines[i] if i < len(lines) else ""
        stripped = line.strip()

        # Handle preprocessor directives.
        if stripped.startswith("#"):
            preprocessor_continuation = stripped.endswith("\\")
            continue

        if preprocessor_continuation:
            preprocessor_continuation = stripped.endswith("\\")
            continue

        # Only inspect declarations at global scope.
        if brace_depth == 0:

            # Ignore completely empty lines before a declaration.
            if not stripped:
                continue

            if declaration_start is None:
                declaration_start = i

            declaration += " " + stripped

            # A semicolon before an opening brace means this is
            # probably a prototype, typedef, variable, or declaration.
            if ";" in declaration and not ("{" in declaration):

                declaration = ""
                declaration_start = None

            elif "(" in declaration:

                # Find the first opening parenthesis.
                opening_paren = declaration.find("(")

                closing_paren = find_matching_paren(
                    declaration,
                    opening_paren,
                )

                if closing_paren is not None:

                    after_paren = declaration[
                        closing_paren + 1:
                    ]

                    # A function definition must have an opening
                    # brace after the parameter list.
                    if "{" in after_paren:

                        before_paren = declaration[
                            :opening_paren
                        ]

                        match = FUNCTION_RE.search(
                            before_paren + "("
                        )

                        if match:

                            function_name = match.group(
                                "name"
                            )

                            # Ignore control-flow constructs such as
                            # if (...), while (...), and for (...).
                            if function_name not in CONTROL_KEYWORDS:

                                functions.append(
                                    {
                                        "name": function_name,
                                        "line": declaration_start,
                                        "static": (
                                            match.group("static")
                                            is not None
                                        ),
                                    }
                                )

                        # The opening brace starts the function body.
                        declaration = ""
                        declaration_start = None

        # Count braces from the cleaned source.
        #
        # Comments and strings have already been removed, so braces
        # inside them cannot affect nesting.
        brace_depth += line.count("{")
        brace_depth -= line.count("}")

        if brace_depth < 0:
            brace_depth = 0

    return lines, functions


def check_file(source_path: Path):
    """Check all functions and comments in a C source file."""

    errors = []

    try:
        original_text = source_path.read_text(
            encoding="utf-8"
        )
    except (UnicodeDecodeError, OSError) as exc:
        print(
            f"Warning: could not read "
            f"{source_path}: {exc}",
            file=sys.stderr,
        )
        return [], []

    all_lines = original_text.splitlines()

    for i, line in enumerate(all_lines):
        line = line.strip()
        if line.startswith("///") and not line.endswith(".") and not line.startswith("/// @file") and all_lines[i+1].startswith('/// @'):
            errors.append(
                f"{source_path}:{i+1}: "
                f"No ending full stop."
            )

    # now funcs

    lines, functions = find_functions(
        source_path
    )

    for function in functions:

        name = function["name"]
        line = function["line"]
        is_static = function["static"]

        # Static functions are private to the source file,
        # so their documentation belongs in the .c file.
        if is_static:

            if not has_doxygen_comment(
                lines,
                line,
            ):
                errors.append(
                    f"{source_path}:{line + 1}: "
                    f"static function '{name}' "
                    f"has no Doxygen comment"
                )

            continue

        # main() generally has no public header declaration.
        if name == "main":

            if not has_doxygen_comment(
                lines,
                line,
            ):
                errors.append(
                    f"{source_path}:{line + 1}: "
                    f"function 'main' "
                    f"has no Doxygen comment"
                )

            continue

        # Non-static functions are considered public.
        # They must have a declaration in a header.
        header = find_header(name)

        if header is None:

            errors.append(
                f"{source_path}:{line + 1}: "
                f"public function '{name}' "
                f"has no header declaration"
            )

        elif not header_has_documentation(
            header,
            name,
        ):

            errors.append(
                f"{source_path}:{line + 1}: "
                f"public function '{name}' "
                f"is not documented in {header}"
            )

    return errors


def main():
    """Run the documentation checker."""

    if not SRC_DIR.exists():

        print(
            f"Error: source directory "
            f"'{SRC_DIR}' does not exist.",
            file=sys.stderr,
        )

        return 1

    if not INCLUDE_DIR.exists():

        print(
            f"Warning: include directory "
            f"'{INCLUDE_DIR}' does not exist.",
            file=sys.stderr,
        )

    errors = []

    for source_file in sorted(
        SRC_DIR.rglob("*.c")
    ):

        errors.extend(
            check_file(source_file)
        )

    if errors:

        print(
            "Undocumented functions found:\n"
        )

        for error in errors:
            print(f"  {error}")

        print(
            f"\n{len(errors)} "
            f"documentation issue(s) found."
        )

        return 1

    print(
        "All functions are documented."
    )

    return 0


if __name__ == "__main__":
    sys.exit(main())