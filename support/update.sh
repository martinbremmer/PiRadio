#!/bin/bash

#
# Just a convenience script that'll update this repository,
# and rebuilds PiRadio.
# This can be added to the Raspian main menu for a relative
# easy PiRadio update on the small touchscreen.
#

# Get dir of this script, which is also the source location.
SCRIPT_DIR=$(dirname "$(readlink -f "$BASH_SOURCE")")

# Be sure we're in the git directory
cd ${SCRIPT_DIR}

# Update code
git pull

# Update application
./build.sh

