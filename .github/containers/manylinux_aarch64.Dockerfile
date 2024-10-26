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
    openblas openblas-devel

ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/:/usr/lib64/" \
    OpenBLAS_DIR="/usr/lib64/"

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

RUN git config --global --add safe.directory "*"
