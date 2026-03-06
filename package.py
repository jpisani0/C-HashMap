#!/usr/bin/env python

# NAME: package.py
# AUTHOR: J. Pisani
# DATE: 3/5/26
#
# DESCRIPTION: Packages the library for distribution

import argparse
import shutil
import sys
import subprocess
from pathlib import Path
import zipfile
import tarfile

LIB_NAME = "hashmap.lib" if sys.platform == "win64" else "libhashmap.a"
DIST_DIR = Path("dist")
PACKAGE_DIR = DIST_DIR.joinpath("hashmap")
LIB_DIR = PACKAGE_DIR.joinpath("lib")
BUILD_DIR = Path("build")
INCLUDE_DIR = Path("include")


# Compress for distribution with ZIP for windows
def zip():
    zip_name = "hashmap-windows-x86_64.zip"

    with zipfile.ZipFile(zip_name, "w", zipfile.ZIP_DEFLATED) as zipf:
        for path in PACKAGE_DIR.rglob("*"):
            zipf.write(path, path.relative_to(DIST_DIR))

    shutil.move(zip_name, DIST_DIR.joinpath(zip_name))


# Compress for distribution with tar for linux
def tar():
    tar_name = "hashmap-linux-x86_64.tar.gz"

    with tarfile.open(tar_name, "w:gz") as tar:
        tar.add(PACKAGE_DIR, arcname="hashmap")

    shutil.move(tar_name, DIST_DIR.joinpath(tar_name))


def main():
    parser = argparse.ArgumentParser(prog="package", description=f"Package the {LIB_NAME} project for distribution")

    parser.add_argument("--clean", action="store_true", help="Clean the distribution package")

    args = parser.parse_args()

    # Clean dist
    if DIST_DIR.exists():
        shutil.rmtree(DIST_DIR)

    # Clean build
    if BUILD_DIR.exists():
        shutil.rmtree(BUILD_DIR)

    # Exit now if just cleaning
    if args.clean:
        sys.exit()

    # Build the library
    subprocess.run(
        ["cmake", "-S", ".", "-B", BUILD_DIR, "-DCMAKE_BUILD_TYPE=Release"],
        check=True
    )

    subprocess.run(
        ["cmake", "--build", BUILD_DIR],
        check=True
    )

    # Create the dirs
    DIST_DIR.mkdir()
    PACKAGE_DIR.mkdir()
    LIB_DIR.mkdir()

    # Copy the library
    lib = BUILD_DIR / "lib" / LIB_NAME
    shutil.copy2(lib, LIB_DIR.joinpath(LIB_NAME))

    # Copy the API header
    shutil.copytree(INCLUDE_DIR, PACKAGE_DIR / INCLUDE_DIR)

    # Copy the README
    shutil.copy2("README.md", PACKAGE_DIR)

    # Copy the License
    shutil.copy2("LICENSE", PACKAGE_DIR)

    # Compress for upload
    if LIB_NAME.endswith(".lib"):
        # Windows
        zip()
    else:
        # Linux
        tar()


if __name__ == "__main__":
    main()
