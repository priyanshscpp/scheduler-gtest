FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    clang \
    clang-format \
    clang-tidy \
    libgtest-dev \
    nlohmann-json3-dev \
    libcli11-dev \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy source code
COPY . .

# Build project
RUN mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build . \
    && cmake --install . --prefix /usr/local

# Set entrypoint
ENTRYPOINT ["/usr/local/bin/cpu-scheduler"]
CMD ["--help"] 