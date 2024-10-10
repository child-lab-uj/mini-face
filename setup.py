"""Build setup

Setup the build environment for a C++ Python extension module.
C++ dependencies can be managed by vcpkg if `VCPKG_ROOT` environmental variable is set.
It uses `scikit-build` to build the C++ extension module.

Project metadata is read from `pyproject.toml`, however `vcpkg.json` is verified too.
"""

# Based on https://github.com/mcleantom/scikit_build_core_vcpkg

import json
from pathlib import Path
from shutil import rmtree
from typing import Any

import tomllib
from setuptools import find_packages
from skbuild import setup

PROJECT_NAME: str
PROJECT_VERSION: str
PROJECT_SOURCE_DIR = Path(__file__).parent
PYPROJECT_MANIFEST = PROJECT_SOURCE_DIR / "pyproject.toml"
VCPKG_MANIFEST = PROJECT_SOURCE_DIR / "vcpkg.json"
SKBUILD_DIR = PROJECT_SOURCE_DIR / "_skbuild"
PYTHON_PACKAGES_ROOT = "src/python"

# For some reason, running this file twice in a row causes the build to fail:
# fatal error C1083: Cannot open include file: 'io.h'
# Therefore the workaround is to clean the `_skbuild` directory before running
if SKBUILD_DIR.exists():
    print(f"Removing previous installation: {SKBUILD_DIR}")
    rmtree(str(SKBUILD_DIR))

# Name and version are read from `pyproject.toml`
with open(PYPROJECT_MANIFEST, mode="rb") as file:
    manifest: dict[str, Any] = tomllib.load(file)
    project: dict[str, Any] = manifest["project"]

    PROJECT_VERSION = project["version"]
    PROJECT_NAME = project["name"]

# Versions and names must be identical in both C++ and Python manifests
with open(VCPKG_MANIFEST, mode="r") as f:
    vcpkg_json: dict[str, Any] = json.load(f)
    cpp_project_version: str = vcpkg_json["version-semver"]
    cpp_project_name: str = vcpkg_json["name"]

    if cpp_project_version != PROJECT_VERSION:
        raise RuntimeError(
            f"Project version mismatch - pyproject.toml: {PROJECT_VERSION}, vcpkg.json: {cpp_project_version}"
        )

    if cpp_project_version != PROJECT_VERSION:
        raise RuntimeError(
            f"Project name mismatch - pyproject.toml: {PROJECT_NAME}, vcpkg.json: {cpp_project_name}"
        )

packages = find_packages(PYTHON_PACKAGES_ROOT)

# Python packages are going to be placed here
package_dir = {"": PYTHON_PACKAGES_ROOT}

# Compiled C++ code in a form of a shared object is going to be placed in this directory
cmake_install_dir = PYTHON_PACKAGES_ROOT + "/mini_face"

cmake_args = [
    "-DBUILD_TESTS=OFF",
]

package_data = {
    "": ["vcpkg.json"],  # ignored if vcpkg is not used (e.g. macOS CI/CD)
}

setup(
    name=PROJECT_NAME,
    version=PROJECT_VERSION,
    packages=packages,
    package_dir=package_dir,
    cmake_install_dir=cmake_install_dir,
    # CMake must always be used, otherwise C++ dependencies won't be installed
    # ! setup_requires=["cmake"] causes `vcpkg` to fail
    cmake_with_sdist=True,
    cmake_args=cmake_args,
    # Add additional data to the package
    include_package_data=True,
    package_data=package_data,
)
