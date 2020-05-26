#!/bin/bash

set -e

# Set VV in .travis.yml to make scripts verbose
[ "$VV" ] && set -x

CACHEDIR=${CACHEDIR:-${HOME}/.cache}

# source functions
. ./.ci/travis/utils.sh

# The module to be built isn't in the cache directory with the dependencies
pwd

# Add the path to the driver module to the RELEASE.local file, since it is needed by the example IOC
update_release_local MOTOR_NEWPORT $TRAVIS_BUILD_DIR

# Copy the travis RELEASE.local to the configure dir
cp -f ${CACHEDIR}/RELEASE.local configure/RELEASE.local

# Sanity check
echo -e "${ANSI_BLUE}Contents of updated configure/RELEASE.local${ANSI_RESET}"
cat configure/RELEASE.local
echo -e "${ANSI_BLUE}End of updated configure/RELEASE.local${ANSI_RESET}"

# Enable the building of example IOCs
echo -e "${ANSI_BLUE}Creating configure/CONFIG_SITE.local${ANSI_RESET}"
echo -e "BUILD_IOCS = YES" > configure/CONFIG_SITE.local
cat configure/CONFIG_SITE.local
echo -e "${ANSI_BLUE}End of configure/CONFIG_SITE.local${ANSI_RESET}"
