# Inspired by https://github.com/geopandas/pyogrio

FROM --platform=linux/arm64 quay.io/pypa/manylinux_2_28_aarch64

# Required dependencies installed by system manager:
#   * general: cmake, ninja-build
#   * mini-face: openblas
#   * pybind11: python3*
#   * python311: autoconf, autoconf-archive, automake
RUN yum -y install \
    curl \
    zip unzip tar \
    autoconf autoconf-archive automake cmake ninja-build \
    libtool pkg-config \
    python311 \
    openblas openblas-devel \
    perl-open \
    perl-IPC-Cmd \
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

ENV VCPKG_DEFAULT_TRIPLET="x64-linux"

# mkdir & touch -> workaround for https://github.com/microsoft/vcpkg/issues/27786
RUN bootstrap-vcpkg.sh && \
    mkdir -p /root/.vcpkg/ $HOME/.vcpkg && \
    touch /root/.vcpkg/vcpkg.path.txt $HOME/.vcpkg/vcpkg.path.txt && \
    vcpkg integrate install && \
    vcpkg integrate bash

COPY vcpkg.json opt/vcpkg/

ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/:/opt/vcpkg/installed/x64-linux/lib"

RUN vcpkg install \
    --feature-flags="versions,manifests" \
    --x-manifest-root=opt/vcpkg \
    --x-install-root=opt/vcpkg/installed && \
    vcpkg list

# setting git safe directory is required for properly building wheels when
# git >= 2.35.3
RUN git config --global --add safe.directory "*"