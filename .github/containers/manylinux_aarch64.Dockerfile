# Inspired by https://github.com/geopandas/pyogrio

FROM quay.io/pypa/manylinux_2_28_aarch64:2024-08-12-7fde9b1

# Required system dependencies:
#   * libepoxy: libx11-dev libgles2-mesa-dev
#   * libxcrypt: autoconf automake libtool pkg-config
#   * python3: autoconf autoconf-archive automake
#   * openssl: perl-ICP-Cmd
RUN yum -y install \
    curl zip unzip tar \
    cmake ninja-build \
    autoconf autoconf-archive automake libtool pkg-config \
    perl-IPC-Cmd \
    python311 \
    opencv opencv-devel \
    openblas openblas-devel \
    libgfortran

RUN git clone https://github.com/davisking/dlib.git /opt/dlib && \
    git -C /opt/dlib checkout tags/v19.24.6

WORKDIR /opt/dlib

RUN mkdir build

WORKDIR /opt/dlib/build

RUN cmake .. && \
    cmake --build .

RUN ls -r dlib

WORKDIR /opt/dlib/build/dlib

RUN make install

ENV LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/lib/:/opt/vcpkg/installed/x64-linux/lib"

# setting git safe directory is required for properly building wheels when
# git >= 2.35.3
RUN git config --global --add safe.directory "*"
