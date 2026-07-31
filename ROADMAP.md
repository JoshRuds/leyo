# Leyo Roadmap

Leyo is a simple alternative to systems languages (like C and Rust) and it should be
safe and easy. 

## Requirements

Leyo must have addons for native assemblies. It should be simple and intuitive to
be used in classrooms and used to teach easily.

## Structure

Leyo is designed to run anywhere and be good at it. A Leyo program should be easy to run on a user's computer, easy to share as a `.lybc` file, and simple to deploy to specialised platforms such as microcontrollers.

The core Leyo compiler produces **Leyo Bytecode (`.lybc`)**, providing a common format for distributing and executing Leyo programs.

A graph demonstates the general idea:

                              Leyo Source
                                   │
                                   ▼
                                 Lexer
                                   │
                                   │ Token Stream
                                   ▼
                                 Parser
                                   │
                                   │ Bytecode
                                   ▼
                         ┌───────────────────┐
                         │                   │
                         ▼                   ▼
                  Device-Specific VM   Native Backend
                         │                   │
                         │                   │
                         ▼                   ▼
                       Run on        Native Executable
                      simulated              │
                      hardware               ▼
                                  ┌────────────────────┐
                                  │                    │
                                  ▼                    ▼
                             Desktop OS        Microcontroller
                            e.g. Windows       e.g. micro:bit
                                                     │
                                                     ▼
                                          Microcontroller Addon
                                                     │
                                    ┌────────────────┼────────────────┐
                                    │                │                │
                                    ▼                ▼                ▼
                              Native APIs         Assembly     Flashing Tools
                                    │            Transpiler           │
                                    │                │                │
                                    └────────────────┼────────────────┘
                                                     ▼
                                               Microcontroller
                                                     │
                                                     ▼
                                                    Run

### Universal Bytecode

The `.lybc` format is the standard way to share compiled Leyo programs. A user should be able to distribute a Leyo application as a single `.lybc` file and run it easily using the Leyo runtime.

This provides a simple distinction between **writing Leyo programs** and **building Leyo itself**. Users should not need a full development environment simply to run a Leyo program.

The same `.lybc` program can also be used as the input for platform-specific SDKs. Depending on the target, it could be packaged into a Windows `.exe`, a Linux executable, a macOS application, or firmware such as a micro:bit `.hex` file.

### Native SDKs

For users who want to target a specific platform, Leyo provides a simple way to install the required native SDK.

A native SDK contains the tools required to convert Leyo bytecode into a format suitable for its target platform. This may include a native compiler, assembly transpiler, linker, platform libraries, and flashing tools.

For example:

```text
program.lybc
      │
      ▼
Micro:bit SDK
      │
      ├── Native Backend
      ├── Assembly Transpiler
      ├── Linker
      └── Flashing Tool
      │
      ▼
program.hex
      │
      ▼
   Micro:bit
```

This means that a user can start with the standard Leyo installation and only install additional SDKs when they need to target a specific platform.

### Community-Driven Addons

Leyo's platform ecosystem is designed to be community-driven. Anyone should be able to create an addon for a new operating system, processor architecture, microcontroller, or other target without modifying the Leyo compiler itself.

Addons should follow a simple and consistent interface, allowing them to integrate with the Leyo executable and its build system. An addon can provide the tools required to take `.lybc` files and produce the appropriate output for its target platform.

For example, an addon could define:

* The target architecture and platform.
* The native compiler or transpiler to use.
* The format of the final output.
* Required platform libraries.
* How the output is packaged.
* How programs are deployed or flashed to hardware.

The goal is that creating an addon should be straightforward enough for the community to support new platforms as they appear.

### Package Management

Leyo may provide an in-built package manager for discovering, installing, updating, and managing addons and SDKs.

This could allow a user to install a new target with a simple command, for example:

```bash
leyo package install microbit
```

After installation, the new target would become available through the standard Leyo executable.

The exact implementation of the package system may evolve, but the underlying principle remains the same: **Leyo should make targeting a new platform as simple as installing the required addon.**

### The Goal

Leyo aims to provide a single, consistent development experience across platforms:

**Write once. Compile once. Run anywhere.**

Whether the final result is a `.lybc` file running in the Leyo VM, a standalone Windows `.exe`, or firmware flashed onto a microcontroller, the user should interact with the same core Leyo language and toolchain.

The complexity of each target should be handled by its SDK or addon, allowing Leyo itself to remain simple, portable, and accessible while still being capable of targeting almost any platform.
