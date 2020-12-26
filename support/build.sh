#!/bin/bash

#
# Just a convenience build script so that QtCreator
# doesn't have to be started on Raspian.
# Which saves some hassle.
#

# Get dir of this script, which is also the source location.
SOURCE_DIR=$(dirname "$(readlink -f "$BASH_SOURCE")")/..

# Prepare the build directory
BUILD_DIR=/home/pi/piradio
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Prepare Makefile through qmake
/usr/lib/qt5/bin/qmake ${SOURCE_DIR}/PiRadio.pro -spec linux-g++ && /usr/bin/make qmake_all

# Actually build the product.
make -j3

