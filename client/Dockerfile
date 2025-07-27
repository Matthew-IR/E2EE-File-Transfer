FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive


# Update package lists and install necessary tools
# g++: The C++ compiler
# cmake: A modern build system for C++
# libssl-dev: The OpenSSL library for encryption
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    make \
    libssl-dev \
    && apt-get clean


WORKDIR /app

COPY common /app/common

COPY client /app/client

# Compile
WORKDIR /app/client
RUN make

CMD ["tail", "-f", "/dev/null"]