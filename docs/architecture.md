# olymp-sandbox Architecture

## Overview

```
  Config ──→ ProcessManager ──→ ResultInfo
                  │
                  ↓
          Platform Layer
        (linux.cpp / windows.cpp)
```

The system follows a linear pipeline:

1. **Config** — parse CLI arguments into a typed configuration
2. **ProcessManager** — spawn a child process, monitor its resource usage, enforce limits
3. **ResultInfo** — collect and serialize execution results

## Namespace: `mc`

All public types live in the `mc` namespace.

## Components

### `mc::config` (`src/config.hpp`, `src/config.cpp`)

A struct that holds all parameters for a single managed execution:

| Field | Type | Default | Description |
|-------|------|---------|-------------|
| `application` | `std::string` | — | Path to the executable |
| `input` | `std::string` | `"input.txt"` | Input file path |
| `output` | `std::string` | `"output.txt"` | Output file path |
| `memory_limit` | `size_t` | 8 000 000 | Memory limit in bytes |
| `time_limit` | `size_t` | 1000 | Time limit in milliseconds |

Key method: `bool init(int argc, char **argv)` — parses `--key=value` CLI arguments.

### `mc::process_manager` (`src/process_manager.hpp`, `src/process_manager.cpp`)

The core orchestrator. Manages the lifecycle of a single child process.

**Constructor** takes a `config` and optional `log_path`. Validates:
- Application path is not empty
- Memory/time limits are non-zero
- Input file exists and is not empty

**Public methods:**

| Method | Returns | Description |
|--------|---------|-------------|
| `start_app()` | `result_info` | Spawns child, monitors resources, returns result |

**Lifecycle (`start_app`):**

```
create_process()
     │
     ▼
while is_process_up():
  ├── is_time_limit?    → TIME_LIMIT, close
  ├── get_process_memory()
  ├── is_memory_limit?  → MEMORY_LIMIT, close
  └── continue polling
     │
     ▼
close_process()
     │
     ▼
validate output file  → RUNTIME_ERROR if missing
     │
     ▼
return result_info
```

**Private methods:**

| Method | Description |
|--------|-------------|
| `create_process()` | Delegates to platform-specific `start_process()` |
| `close_process(pid)` | Delegates to `stop_process()`, returns success |
| `is_memory_limit(pid, limit)` | Returns true if process exceeds memory limit |
| `is_time_limit(pid, start_time)` | Returns true if process exceeds time limit |
| `is_process_up(pid)` | Delegates to `is_up_process()` |

### `mc::result_info` (`src/result_info.hpp`, `src/result_info.cpp`)

A struct representing the outcome of an execution.

**Status codes:**

| Value | Meaning |
|-------|---------|
| `UNKNOWN` | Initial state before execution |
| `OK` | Process completed within limits |
| `TIME_LIMIT` | Exceeded time limit, was killed |
| `MEMORY_LIMIT` | Exceeded memory limit, was killed |
| `RUNTIME_ERROR` | Process failed to start or exited abnormally |

**Fields:**

| Field | Type | Description |
|-------|------|-------------|
| `status_code` | `STATUS` | Execution outcome |
| `config` | `mc::config` | Configuration used for this run |
| `max_memory_used` | `size_t` | Peak memory usage in bytes |
| `time_used` | `int64_t` | Wall-clock runtime in milliseconds |

**Key method:** `std::string JSON() const` — serializes all fields as JSON.

### `mc::logger` (`src/logger.hpp`, `src/logger.cpp`)

Simple file-based logger.

**Log levels:**
- `debug` — detailed diagnostic output
- `info` — normal operational messages
- `warning` — recoverable issues
- `error` — non-recoverable issues

Output format: `[level] message`

### Platform Layer (`src/linux.cpp`, `src/windows.cpp`)

One of these files is compiled depending on the target OS (detected via CMake's `CheckIncludeFiles`).

| Function | Linux | Windows |
|----------|-------|---------|
| `start_process(filename, input, output)` | `fork()` + `execvp()`, stdin/stdout via `dup2()` | `CreateProcess()` with `CreateFile` handles, job object |
| `get_process_memory(pid)` | Reads `/proc/pid/status` VmSize | `GetProcessMemoryInfo()` WorkingSetSize |
| `is_up_process(pid)` | `kill(pid, 0)` | `GetExitCodeProcess()` → STILL_ACTIVE |
| `stop_process(pid)` | `kill(pid, SIGTERM)` | `TerminateProcess()` + `CloseHandle()` |
| `get_current_time()` | `clock_gettime()` / `gettimeofday()` | `GetTickCount64()` |

On Windows, the child process is assigned to a **Job Object** (`JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE`) so that all child processes are terminated when the job handle is closed.

## Directory Structure

```
.
├── CMakeLists.txt
├── Dockerfile
├── examples/
│   └── testapp/testapp.cpp      # Test application for integration tests
├── src/
│   ├── config.hpp / .cpp        # Configuration
│   ├── definitions.hpp          # Shared types, process_id_t
│   ├── linux.cpp                # Linux platform implementation
│   ├── windows.cpp              # Windows platform implementation
│   ├── logger.hpp / .cpp        # Logging
│   ├── olymp-sandbox.cpp        # Entry point
│   ├── process_manager.hpp/ .cpp # Core orchestrator
│   └── result_info.hpp / .cpp   # Execution results
├── tests/
│   ├── test_config.cpp          # Config unit tests
│   ├── test_definitions.cpp     # Definitions unit tests
│   ├── test_integration.cpp     # Integration tests (Linux)
│   ├── test_process_manager.cpp # ProcessManager unit + edge case tests
│   └── test_result_info.cpp     # ResultInfo unit tests
├── docs/
│   ├── architecture.md          # Architecture overview
│   ├── memory_measurement_1.md  # Windows memory measurement research
│   ├── memory_measurement_2.md  # Cross-platform memory/CPU measurement
│   └── thread_measurement.md    # Thread time measurement research
├── .github/workflows/build.yml  # CI pipeline
└── .dockerignore
```

## Test Strategy

| Layer | Test file | Type | Platform |
|-------|-----------|------|----------|
| Config | `test_config.cpp` | Unit (stubs) | All |
| Definitions | `test_definitions.cpp` | Unit (stubs) | All |
| ResultInfo | `test_result_info.cpp` | Unit (stubs) | All |
| ProcessManager | `test_process_manager.cpp` | Unit (stubs + edge cases) | All |
| Integration | `test_integration.cpp` | Integration (real OS) | Linux |

- **Unit tests** stub out OS functions (`start_process`, `is_up_process`, etc.) to test business logic in isolation.
- **Integration tests** use the real platform implementation with `testapp` as the child process.
- Tests are registered with Catch2 and discovered automatically via `ctest`.
