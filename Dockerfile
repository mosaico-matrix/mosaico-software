# Use Ubuntu base image
FROM ubuntu:latest

# Configure timezone data before installing other packages
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y tzdata && \
    ln -fs /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    python3-venv \
    python3-pybind11 \
    clang \
    libx11-dev \
    libcurl4-openssl-dev

# Set working directory
WORKDIR /app

# Copy project files
COPY . /app

WORKDIR /app

# Expose the necessary ports
EXPOSE 5000
EXPOSE 5683/udp


CMD ["./start-web.sh"]
