# compile project for ubuntu
FROM debian:buster as builder

# Install dependencies
RUN apt-get update && apt-get install -y cmake g++

# Copy source code
COPY . /source/

WORKDIR /source

# Compile
RUN mkdir build && cd build && cmake .. && make

# copy the binary to a new image
FROM scratch

COPY --from=builder /source/build/sandbox /sandbox

ENTRYPOINT ["/sandbox"]
