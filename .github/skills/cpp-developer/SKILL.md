---
name: cpp-developer
description: Build high-performance C++ applications. Use for systems programming, performance-critical code, embedded development, game development, and native library integration.
---

# C++ Developer

## Description
A skill for high-performance C++ development, including modern language features, memory safety patterns, concurrency, and systems-level optimization.

## Priority Rules
Prioritize in this order when trade-offs conflict:
1. Memory safety and correctness
2. Performance and efficiency
3. Maintainability and modern C++ practices
4. Portability

## When to Use
- Building performance-critical applications
- Systems programming and native tooling
- Embedded or real-time components
- Optimizing CPU and memory usage
- Game, graphics, or compute-heavy workloads
- Integrating native C/C++ libraries

## Instructions
1. **Plan architecture** - define modules, ownership, and interfaces
2. **Use safe memory patterns** - RAII and smart pointers
3. **Write modern C++** - C++17/C++20 best practices
4. **Leverage STL** - containers, algorithms, iterators
5. **Test behavior** - unit tests with GoogleTest/Catch2
6. **Profile before tuning** - optimize real bottlenecks
7. **Document APIs** - concise Doxygen comments where needed

## Input Recovery Rules
- Assume modern C++17/20 when standard version is not specified
- Ask for clarification only when performance requirements or target platform significantly affects the approach

## Constraints
- Do not use raw pointers without explicit justification
- Do not skip RAII patterns for resource management
- Do not optimize before profiling

## Technologies and Standards
- Modern C++ (C++17, C++20)
- RAII, `std::unique_ptr`, `std::shared_ptr`
- STL containers and algorithms
- Concurrency: `std::thread`, `std::mutex`, atomics
- Build systems: CMake, Make
- Toolchains: GCC, Clang, MSVC
- Libraries: Boost, Qt, OpenCV, Eigen
