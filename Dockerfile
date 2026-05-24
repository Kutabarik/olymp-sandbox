# olymp-sandbox — multistage build
# Usage:
#   docker build --target builder -t olymp-sandbox .
#   docker build --target test -t olymp-sandbox-test .
#   docker run olymp-sandbox-test
#   docker build --target release -t olymp-sandbox-release .

# Stage 1: Build all targets
FROM debian:bookworm AS builder

RUN apt-get update && apt-get install -y cmake g++ make

WORKDIR /source
COPY . .

RUN cmake -S . -B build && cmake --build build --parallel

# Stage 2: Run tests
FROM builder AS test

CMD ctest --test-dir build --output-on-failure

# Stage 3: Release binary
FROM scratch AS release

COPY --from=builder /source/build/sandbox /sandbox

ENTRYPOINT ["/sandbox"]
