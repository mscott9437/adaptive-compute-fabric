FROM nvidia/cuda:12.4.1-devel-ubuntu22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    gcc \
    g++ \
    cmake \
    ninja-build \
    git \
    curl \
    python3 \
    python3-pip \
    pkg-config

WORKDIR /workspace

CMD ["/bin/bash"]