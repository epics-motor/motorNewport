#!/usr/bin/env python

import os
import shutil
# ugly hack: copy cue.py so that it can be imported
shutil.copy('.ci/cue.py', '.ci-local/travis')
from cue import *

# Add the path to the driver module to the RELEASE.local file, since it is needed by the example IOC
update_release_local('MOTOR_NEWPORT', os.getenv('TRAVIS_BUILD_DIR'))

# Copy the travis RELEASE.local to the configure dir
shutil.copy("{}/RELEASE.local".format(cachedir), "configure/RELEASE.local")

# Sanity check
print("{}Contents of updated configure/RELEASE.local{}".format(ANSI_BLUE, ANSI_RESET))
os.system('cat configure/RELEASE.local')
print("{}End of updated configure/RELEASE.local{}".format(ANSI_BLUE, ANSI_RESET))

# Enable the building of example IOCs
print("{}Contents of updated configure/CONFIG_SITE.local{}".format(ANSI_BLUE, ANSI_RESET))
os.system('echo "BUILD_IOCS = YES" > configure/CONFIG_SITE.local')
os.system('cat configure/CONFIG_SITE.local')
print("{}End of updated configure/CONFIG_SITE.local{}".format(ANSI_BLUE, ANSI_RESET))

# Remove cue.py
os.system('rm -f .ci-local/travis/cue.py')
