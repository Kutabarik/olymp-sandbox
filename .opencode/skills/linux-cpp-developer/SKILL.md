---
name: linux-cpp-developer
description: Build advanced C++ software for Linux. Derived from cpp-developer. Use for POSIX/Linux-native systems programming, object-oriented architecture, and production-grade native applications following Google C++ Style Guide with Catch2 unit testing.
---

# Linux C++ Developer

## Description
A derived skill from C++ Developer for advanced native software development on Linux with an object-oriented architecture preference, modern C++ practices, and production reliability.

## Priority Rules
Prioritize in this order when trade-offs conflict; if priorities collide, prefer the higher item unless explicit product requirements accept the trade-off (for example, a bounded performance regression to preserve correctness).
1. Correctness and memory safety
2. Linux platform correctness and API compatibility
3. Maintainable architecture with clear ownership and interface boundaries
4. Performance and resource efficiency
5. Portability where feasible

## When to Use
- Developing production-grade native applications for Linux
- Building system tools, daemons, CLI utilities, and backend services in C++
- Integrating with POSIX/Linux APIs, filesystems, and process primitives
- Refactoring legacy C++ codebases into maintainable OOP designs
- Creating and maintaining unit-tested C++ modules with Catch2

## Instructions
1. **Define Linux scope** - target distribution, compiler, runtime, and deployment constraints
2. **Design OOP architecture** - model domain with clear interfaces, ownership boundaries, and SOLID-oriented class design
3. **Implement with modern C++** - apply RAII, smart pointers, value semantics, and strong typing
4. **Use Linux-native capabilities** - integrate required POSIX/Linux APIs with robust error handling
5. **Follow style conventions** - apply Google C++ Style Guide consistently in naming, file layout, and API shape
6. **Test with Catch2** - create focused unit tests for behavior, boundaries, and failure paths
7. **Profile and optimize** - tune only measured bottlenecks in CPU, memory, and I/O
8. **Document architecture** - capture module contracts, key decisions, and platform assumptions

## Input Recovery Rules
- Assume modern C++17 or newer when standard version is not specified
- Assume OOP-first architecture unless specific performance, memory-layout, hardware-interaction, or kernel/API constraints require procedural or data-oriented alternatives
- Ask for clarification only when Linux target, API surface, or non-functional requirements materially change design decisions

## Constraints
- Do not bypass RAII or lifetime-safe ownership patterns
- Avoid direct ownership with raw `new`/`delete` except in tightly constrained low-level integrations
- Do not introduce platform-specific calls without isolating them behind clear interfaces
- Do not deviate from Google C++ Style Guide without explicit project-level exception
- Do not ship modules without unit tests for core behavior and failure scenarios

## Technologies and Standards
- Language: C++17/C++20
- Toolchains: GCC, Clang
- Platform APIs: POSIX, Linux system interfaces
- Testing: Catch2 (https://github.com/catchorg/Catch2)
- Style guide: Google C++ Style Guide (https://google.github.io/styleguide/cppguide.html)
- Build systems: CMake, Make
- Build backends/tools: Ninja

## Best Practices
- Keep platform-specific code in adapter or infrastructure layers
- Prefer composition over deep inheritance chains
- Use exception-safe and resource-safe patterns for file descriptors and system resources
- Prefer explicit, recoverable error handling and preserve actionable diagnostic information
- Prefer well-scoped synchronization and explicit ownership in concurrent code
- Validate behavior with deterministic tests and clear fixtures
- Keep interfaces small, explicit, and mock-friendly for testing
- Include structured logging and actionable diagnostics for operational failures
