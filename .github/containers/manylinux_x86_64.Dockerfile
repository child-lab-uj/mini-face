# Inspired by https://github.com/geopandas/pyogrio

FROM quay.io/pypa/manylinux_2_28_x86_64:2024-08-12-7fde9b1

# Required system dependencies:
#   * libepoxy: libx11-dev libgles2-mesa-dev
#   * libxcrypt: autoconf automake libtool pkg-config
RUN dnf -y install curl zip unzip tar ninja-build pkg-config \
    openblas opencv
# autoconf automake libtool pkg-config libxcrypt \
# libX11 mesa-libEGL-devel

RUN git clone https://github.com/Microsoft/vcpkg.git /opt/vcpkg && \
    git -C /opt/vcpkg checkout tags/2024.09.30

ENV VCPKG_ROOT="/opt/vcpkg"
ENV PATH="${PATH}:/opt/vcpkg"

ENV VCPKG_DEFAULT_TRIPLET="x64-linux"

# Don't build all the bloat from source
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# mkdir & touch -> workaround for https://github.com/microsoft/vcpkg/issues/27786
RUN bootstrap-vcpkg.sh && \
    mkdir -p /root/.vcpkg/ $HOME/.vcpkg && \
    touch /root/.vcpkg/vcpkg.path.txt $HOME/.vcpkg/vcpkg.path.txt && \
    vcpkg integrate install && \
    vcpkg integrate bash

COPY .github/vcpkg_triplets/x64-linux.cmake opt/vcpkg/custom_triplets/x64-linux.cmake
COPY vcpkg.json opt/vcpkg/

ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/opt/vcpkg/installed/x64-linux/lib"

RUN vcpkg install --overlay-triplets=opt/vcpkg/custom_triplets \
    --feature-flags="versions,manifests" \
    --x-manifest-root=opt/vcpkg \
    --x-install-root=opt/vcpkg/installed && \
    vcpkg list

# setting git safe directory is required for properly building wheels when
# git >= 2.35.3
RUN git config --global --add safe.directory "*"
