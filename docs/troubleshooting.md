# Troubleshooting

## Namespace Isolation

### `clone()` fails, falling back to `fork()`

**Cause:** The sandbox requires `CAP_SYS_ADMIN` to create PID, mount, and network namespaces via `clone()`. Without it, `clone()` returns `-EPERM` and the sandbox falls back to `fork()`.

**Solution:**
- Run as root: `sudo ./sandbox ...`
- In Docker, add `--privileged` or `--cap-add=SYS_ADMIN`
- WSL2 users: run from a root shell (`sudo -s`)

### WSL1 does not support namespaces

**Cause:** WSL1 uses a translation layer that does not implement Linux namespace syscalls.

**Solution:** Upgrade to WSL2 (`wsl --set-version <distro> 2`), which runs a full Linux kernel.

### `--security-opt seccomp=unconfined` required in Docker

**Cause:** Docker's default seccomp profile blocks the `clone()` syscall with namespace flags.

**Solution:** Add `--security-opt seccomp=unconfined` alongside `--cap-add=SYS_ADMIN`.

---

## cgroups

### `/sys/fs/cgroup` not found or not writable

**Cause:** cgroup v2 is not mounted or the process lacks write access.

**Solution:**
- On most modern distros, cgroup v2 is mounted by systemd automatically. Verify with `mount | grep cgroup`.
- If missing: `sudo mount -t cgroup2 none /sys/fs/cgroup`
- In Docker, use `--privileged` to mount cgroups inside the container.

### Memory limit not enforced despite setting `--memory`

**Cause:** Without cgroups, the sandbox uses polling (`get_process_memory()`), which reads `/proc/PID/status`. The child process is killed only after the next poll cycle detects the limit was exceeded. A fast allocation burst may exceed the limit before detection.

**Solution:** Run with root privileges so cgroup v2 memory limits are applied. The kernel enforces the limit synchronously when `memory.max` is exceeded.

---

## Test Failures

### Integration tests fail on Windows

**Expected behavior.** Integration tests use Linux-specific APIs (`fork`, `clone`, `/proc`). They are guarded with `SKIP` at runtime.

### Unit tests fail with `RUNTIME_ERROR` status

**Cause:** In the current test suite, `tests/test_process_manager.cpp` already creates and writes the output file in its `start_process` stub. If you see this failure, check for local modifications or an out-of-date branch where that stub no longer creates the expected output file.

### `VmSize` not found in `/proc/PID/status`

**Cause:** The process has exited and become a zombie. Zombie processes have no `VmSize` entry. This was a known issue fixed in Sprint 13 by using `waitpid()` instead of `kill(pid, 0)` for process liveness checks.

**Solution:** Ensure you are using the latest version from `main`.

### `testapp: command not found`

**Cause:** The integration test cannot locate the `testapp` binary. The `TESTAPP_PATH` compile definition or the default `"testapp"` path must point to a valid executable.

**Solution:** Build the `testapp` target first: `cmake --build build --target testapp`

---

## Build

### undefined reference to `std::filesystem::...`

**Cause:** Older GCC versions (before 12) need explicit `-lstdc++fs` linking.

**Solution:** Use GCC >= 12.2, which embeds `std::filesystem` in the standard library.

### CMake cannot find Catch2

**Cause:** Catch2 is fetched via `FetchContent` during CMake configure. Network access is required on first build.

**Solution:** Ensure the build machine has internet access during the first CMake configure. After the first build, Catch2 is cached in `build/_deps/`.

---

## Docker

### `docker build` fails with permission denied

**Cause:** The Dockerfile copies build artifacts that require read permission for the `test` user.

**Solution:** Ensure `chmod` steps in the Dockerfile grant read access to the non-root user.

### Integration tests inside Docker return `SKIP` for cgroup/namespace tests

**Cause:** Without `--privileged`, the container lacks `CAP_SYS_ADMIN` and cgroup access.

**Solution:** Run the container with `--privileged` or `--cap-add=SYS_ADMIN --security-opt seccomp=unconfined`.
