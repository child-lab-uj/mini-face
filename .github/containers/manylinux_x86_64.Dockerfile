# Inspired by https://github.com/geopandas/pyogrio

FROM quay.io/pypa/manylinux_2_28_x86_64

# Required system dependencies:
#   * libepoxy: libx11-dev libgles2-mesa-dev
#   * libxcrypt: autoconf automake libtool pkg-config
#   * python3: autoconf autoconf-archive automake
#   * openssl: perl-ICP-Cmd
RUN yum -y install \
    curl zip unzip tar \
    cmake ninja-build \
    autoconf autoconf-archive automake libtool pkg-config \
    perl-open \
    perl-IPC-Cmd \
    python311 \
    openblas openblas-devel \
    libXi libXi-devel \
    libXt libXt-devel \
    libXtst libXtst-devel \
    libXrandr libXrandr-devel

# Required for libsystemd
RUN python3 -m ensurepip --upgrade && \
    python3 -m pip install jinja2

RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg && \
    git -C /opt/vcpkg checkout tags/2024.09.30

ENV VCPKG_ROOT="/opt/vcpkg"
ENV PATH="${PATH}:/opt/vcpkg"
ENV VCPKG_BUILD_TYPE="release"
ENV VCPKG_DEFAULT_TRIPLET="x64-linux-release"

# mkdir & touch -> workaround for https://github.com/microsoft/vcpkg/issues/27786
RUN bootstrap-vcpkg.sh && \
    mkdir -p /root/.vcpkg/ $HOME/.vcpkg && \
    touch /root/.vcpkg/vcpkg.path.txt $HOME/.vcpkg/vcpkg.path.txt && \
    vcpkg integrate install && \
    vcpkg integrate bash

ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/:/opt/vcpkg/installed/x64-linux/lib:/opt/vcpkg/installed/x64-linux-release/lib"

# setting git safe directory is required for properly building wheels when
# git >= 2.35.3
RUN git config --global --add safe.directory "*"
