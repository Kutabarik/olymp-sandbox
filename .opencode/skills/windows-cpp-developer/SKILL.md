---
name: windows-cpp-developer
description: Build advanced C++ software for Windows. Derived from cpp-developer. Use for Win32 and modern Windows APIs, Windows-native resource and error handling, and production-grade native applications following Google C++ Style Guide with Catch2 unit testing.
---

# Windows C++ Developer

## Description
A derived skill from C++ Developer for advanced native software development on Windows with modern C++ practices, Windows-native engineering semantics, and production reliability.

## Priority Rules
Prioritize in this order when trade-offs conflict; if priorities collide, prefer the higher item unless explicit product requirements accept the trade-off (for example, a bounded performance regression to preserve correctness).
1. Correctness and memory safety
2. Windows platform correctness and API compatibility
3. Maintainable architecture with clear ownership and interface boundaries
4. Performance and resource efficiency
5. Portability where feasible

## When to Use
- Developing production-grade native applications for Windows
- Building Win32, system, desktop, and tooling software
- Integrating with Windows APIs, services, and file/process abstractions
- Refactoring legacy C++ codebases into maintainable OOP designs
- Creating and maintaining unit-tested C++ modules with Catch2

## Instructions
1. **Define Windows scope** - target Windows versions, toolchain, runtime, and deployment constraints
2. **Design OOP architecture** - model domain with clear interfaces, ownership boundaries, and SOLID-oriented class design
3. **Implement with modern C++** - apply RAII, smart pointers, value semantics, and strong typing
4. **Use Windows-native capabilities** - integrate required Windows APIs with robust error handling
5. **Follow style conventions** - apply Google C++ Style Guide consistently in naming, file layout, and API shape
6. **Test with Catch2** - create focused unit tests for behavior, boundaries, and failure paths
7. **Profile and optimize** - tune only measured bottlenecks in CPU, memory, and I/O
8. **Document architecture** - capture module contracts, key decisions, and platform assumptions

## Input Recovery Rules
- Assume modern C++17 or newer when standard version is not specified
- Assume OOP-first architecture unless specific performance, memory-layout, hardware-interaction, UI-thread/message-loop, or ABI constraints require procedural or data-oriented alternatives
- Ask for clarification only when Windows target, API surface, or non-functional requirements materially change design decisions

## Constraints
- Do not bypass RAII or lifetime-safe ownership patterns
- Avoid direct ownership with raw `new`/`delete` except in tightly constrained low-level integrations
- Do not introduce platform-specific calls without isolating them behind clear interfaces
- Do not transfer ownership across DLL boundaries without explicit ABI and allocator guarantees
- Do not deviate from Google C++ Style Guide without explicit project-level exception
- Do not ship modules without unit tests for core behavior and failure scenarios

## Technologies and Standards
- Language: C++17/C++20
- Toolchains: MSVC, Clang-cl
- Platform APIs: Win32, Windows SDK
- Testing: Catch2 (https://github.com/catchorg/Catch2)
- Style guide: Google C++ Style Guide (https://google.github.io/styleguide/cppguide.html)
- Build systems: CMake, MSBuild
- Build backends/tools: Ninja

## Best Practices
- Keep platform-specific code in adapter or infrastructure layers
- Prefer composition over deep inheritance chains
- Encapsulate HANDLE, COM, and other Windows resource lifetimes with RAII wrappers
- Isolate COM initialization, threading models, and interface ownership behind explicit abstractions
- Prefer Unicode-safe APIs and explicit UTF-8/UTF-16 boundary handling
- Preserve actionable Windows error information (`HRESULT`, `GetLastError`, and `NTSTATUS` where relevant)
- Respect UI thread affinity and message-loop constraints in desktop applications
- Validate untrusted input and use secure Windows API patterns for filesystem, process, and registry operations
- Validate behavior with deterministic tests and clear fixtures
- Keep interfaces small, explicit, and mock-friendly for testing
- Include structured logging and actionable diagnostics for operational failures
