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
    python311


RUN --mount=type=cache,target=/tmp/git_cache/vcpkg \
    git clone https://github.com/Microsoft/vcpkg.git /tmp/git_cache/vcpkg && \
    cp -r /tmp/git_cache/vcpkg /opt/vcpkg

RUN git -C /opt/vcpkg checkout tags/2024.09.30

ENV VCPKG_ROOT="/opt/vcpkg"
ENV PATH="${PATH}:/opt/vcpkg"
ENV VCPKG_DEFAULT_TRIPLET="arm64-linux"
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

RUN bootstrap-vcpkg.sh && \
    mkdir -p /root/.vcpkg/ $HOME/.vcpkg && \
    touch /root/.vcpkg/vcpkg.path.txt $HOME/.vcpkg/vcpkg.path.txt && \
    vcpkg integrate install && \
    vcpkg integrate bash

COPY .github/manifests/linux_aarch64/vcpkg.json /opt/vcpkg/

ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/:/usr/lib64/:/opt/vcpkg/installed/arm64-linux/lib"

# Fixing the 'Detecting CPU failed' issue
ENV VCPKG_CMAKE_CONFIGURE_OPTIONS="-DTARGET=NEHALEM"
ENV VCPKG_BUILD_OPTIONS="--cmake-args=-DTARGET=NEHALEM"

RUN vcpkg install \
    --feature-flags="versions,manifests" \
    --x-manifest-root=opt/vcpkg \
    --x-install-root=opt/vcpkg/installed && \
    vcpkg list

RUN --mount=type=cache,target=/tmp/git_cache/dlib \
    git clone https://github.com/davisking/dlib.git /tmp/git_cache/dlib && \
    cp -r /tmp/git_cache/dlib /opt/dlib

RUN --mount=type=cache,target=/tmp/git_cache/opencv \
    git clone https://github.com/opencv/opencv.git /tmp/git_cache/opencv && \
    cp -r /tmp/git_cache/opencv /opt/opencv

RUN mkdir -p /opt/scripts
COPY --chmod=777 .github/scripts/build_and_install_dependency.sh /opt/scripts

RUN /opt/scripts/build_and_install_dependency.sh dlib && \
    /opt/scripts/build_and_install_dependency.sh opencv

# Link the opencv & dlib libraries
ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/opt/opencv/build/lib:/opt/dlib/build/dlib"

RUN git config --global --add safe.directory "*"
