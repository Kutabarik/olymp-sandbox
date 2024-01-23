# olymp-sandbox

## About
This project is a program for organizing programming Olympiads. This program allows participants to send their solutions as files to the server, where a PHP script compiles them (if necessary) into an executable file.

Next, the C++ program runs the executable file in a separate thread and runs it through predefined tests. Each test verifies the correctness of the input and output data under the constraints set by the condition in execution time and memory consumption.

The results of testing participants' solutions are stored in a database, and participants can see their results on a web page.

This program is designed to organize programming Olympiads and helps automate the process of testing participants' solutions. It allows you to save time and effort of the organizers, and also ensures the accuracy and reliability of testing.

## Project requirements

- gcc >= 12.2
- make
- ...

## Build

The project has simple `Makefile.msys`. For starting, clone project, switch to the project directory and make it.

Make options:

- `make`          - show help
- `make all`      - build and test
- `make build`    - build app
- `make clean`    - remove binary and object files
- `make install`  - install app in environment
- `make pre-test` - compile test application
- `make test`     - run tests
- `make test-all` - compile test app and run tests

```bash
git clone https://github.com/Kutabarik/olymp-sandbox.git

cd olymp-sandbox

make -f Makefile.msys build
```

You can run tests:

```bash
make -f Makefile.msys test 
```

Or clean project:

```bash
make -f Makefile.msys clean 
```

You can also use cmake tool for build.

# Usage

In the `bin` directory `olymp-sandbox` application will be created, this application supports the following command line  (short and long) keys:

- `h` | `help`        - help message
- `a` | `app`         - name of managed application
- `t` | `time`        - max execution time, supported suffixes: milliseconds (ms, default) or seconds (s) or minutes (m)    
- `m` | `memory`      - max memory usage, supported prefixes: bytes (b, default), kilobytes (k) or megabytes (m)
- `i` | `input`       - input data file for application
- `o` | `output`      - output data file for application

Usage sample:

```bash
olymp-sandbox -a /path/to/app -t 1s -m 16m -i /path/to/test/01-input.txt -o path/to/test/01-output.txt
```
