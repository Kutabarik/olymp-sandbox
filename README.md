# olymp-sandbox

Sandbox for securely running programming contest solutions with resource limits.

## Quick Start

### Native (Linux / MSYS2)

```bash
cmake -S . -B build
cmake --build build
./build/sandbox --app=./build/testapp --time=1s --memory=16m --input=input.txt --output=output.txt
```

### Docker

```bash
# Build and run tests
docker build --target test -t olymp-sandbox-test .
docker run olymp-sandbox-test

# Build release image
docker build --target release -t olymp-sandbox-release .
docker run olymp-sandbox-release --app=/testapp --time=1s --memory=16m --input=/input.txt --output=/output.txt
```

## Build Targets

| Target | Description |
|--------|-------------|
| `sandbox` | Main application |
| `testapp` | Test application used in integration tests |
| `run_tests` | Unit test suite (50 tests, stubs OS) |
| `run_integration_tests` | Integration test suite (Linux) |

## Test

```bash
# Unit tests
cmake --build build --target run_tests
ctest --test-dir build --output-on-failure

# Integration tests (Linux only)
cmake --build build --target run_integration_tests
ctest --test-dir build --test-dir build --output-on-failure
```

## CLI Options

| Key | Description | Example |
|-----|-------------|---------|
| `--app` | Path to the executable | `--app=./solution` |
| `--time` | Time limit (ms/s/m) | `--time=2s` |
| `--memory` | Memory limit (b/k/m) | `--memory=64m` |
| `--input` | Input file path | `--input=tests/01.in` |
| `--output` | Output file path | `--output=tests/01.out` |
| `--help` | Show help message | `--help` |

## Requirements

### Build-Time

- CMake >= 3.10
- GCC >= 12.2 (C++20)

### Linux Kernel (for resource isolation)

| Feature | Minimum Kernel | Required Privilege |
|---------|---------------|-------------------|
| PID namespace (`CLONE_NEWPID`) | 2.6.24 | `CAP_SYS_ADMIN` |
| Mount namespace (`CLONE_NEWNS`) | 2.4.19 | `CAP_SYS_ADMIN` |
| Network namespace (`CLONE_NEWNET`) | 2.6.24 | `CAP_SYS_ADMIN` |
| cgroup v2 memory limit | 4.5+ | root |

Isolation features are optional — the sandbox falls back to `fork()` without privileges.

### Runtime Privileges

- **Namespaces** — requires `CAP_SYS_ADMIN` or root. Without it, `start_process()` falls back to `fork()` without isolation.
- **cgroups v2** — requires root and a mounted cgroup v2 filesystem at `/sys/fs/cgroup`. Without it, memory limit enforcement uses polling (`get_process_memory()`), which is softer and less precise.
- **Docker** — use `--privileged` for full isolation, or `--cap-add=SYS_ADMIN --security-opt seccomp=unconfined` for namespaces only.

### Ubuntu / Debian

```bash
sudo apt install cmake g++-12
cmake -S . -B build
cmake --build build
# Run with namespace + cgroup isolation:
sudo ./build/sandbox --app=./build/testapp --time=1s --memory=16m --input=input.txt --output=output.txt
```

### Docker (privileged)

```bash
# Build
docker build --target test -t olymp-sandbox-test .
# Run with full isolation:
docker run --privileged olymp-sandbox-test
# Run with limited capabilities (no cgroups, only namespaces):
docker run --cap-add=SYS_ADMIN --security-opt seccomp=unconfined olymp-sandbox-test
```

## CI

GitHub Actions builds and tests on both Ubuntu (GCC) and Windows (MinGW).

See `.github/workflows/build.yml` for details.

## Architecture

See [docs/architecture.md](docs/architecture.md) for a full description of components, data flow, and test strategy.

## Troubleshooting

See [docs/troubleshooting.md](docs/troubleshooting.md) for common issues and solutions.
