# motorNewport Releases

## __R1-2-1 (2023-05-15)__
R1-2-1 is a release based on the master branch.

### Changes since R1-2

#### Bug fixes
* Pull request [#20](https://github.com/epics-motor/motorNewport/pull/20): Fix for 'make uninstall' removing iocsh files

## __R1-2 (2023-05-11)__
R1-2 is a release based on the master branch.

### Changes since R1-1

#### New features
* Pull request [#14](https://github.com/epics-motor/motorNewport/pull/14) and [#15](https://github.com/epics-motor/motorNewport/pull/15): [Heinz Junkes](https://github.com/hjunkes) Added support for the Newport FCL200 (FC series) to the AG_CONEX driver

#### Modifications to existing features
* Commit [e4e75f7](https://github.com/epics-motor/motorNewport/commit/e4e75f7db0ed7947158c24f070d5eda1f2dc78d5): Removed SUPPORT from RELEASE; use RELEASE.local to define it instead, needed for OPI conversion
* Pull request [#9](https://github.com/epics-motor/motorNewport/pull/9): Added example ESP300 serial port configuration from [Lee Yang](https://github.com/LeeYangLBLBCS)

#### Bug fixes
* Pull request [#18](https://github.com/epics-motor/motorNewport/pull/18): drvESP300.cc now sets both position and encoder_position, so that UEIP of the motor record can be set to YES.

#### Documentation
* Pull request [#17](https://github.com/epics-motor/motorNewport/pull/17): Added SMC100 to README.md

#### Continuous integration
* Added ci-scripts (v3.0.1)
* Switched from Travis CI to Github Actions

## __R1-1 (2020-05-12)__
R1-1 is a release based on the master branch.  

### Changes since R1-0-1

#### New features
* [MarkRivers](https://github.com/MarkRivers) added support for XPS-D which uses sftp rather than ftp to download trajectory files
* Commit [84b3660](https://github.com/epics-motor/motorNewport/commit/84b36607b5cb2596aa98e7847e3f6a321d8b9899): User displays can now be autoconverted at build time

#### Modifications to existing features
* Commit [dee02c8](https://github.com/epics-motor/motorNewport/commit/dee02c8d42d4f9466926c17fb9c8d90b7acd2cea): ``CONFIG_SITE`` now includes ``$(SUPPORT)/configure/CONFIG_SITE``, if it exists
* Commit [787819d](https://github.com/epics-motor/motorNewport/commit/787819d72ace6acb67efb4775b1a89f38af85063): ``SUPPORT`` is now defined in ``RELEASE``, which is needed for OPI autoconvert
* Commit [b116444](https://github.com/epics-motor/motorNewport/commit/b116444e610a13c06f74c2641ff12a189f1d3417): User displays have been autoconverted using the latest converter

#### Bug fixes
* Commit [4cf6214](https://github.com/epics-motor/motorNewport/commit/4cf6214bffc74f0784a2b141a1453f8fb01a05c8): Include ``$(MOTOR)/modules/RELEASE.$(EPICS_HOST_ARCH).local`` instead of ``$(MOTOR)/configure/RELEASE``
* Commit [a2cec4b](https://github.com/epics-motor/motorNewport/commit/a2cec4bd92977b3659f2c9c2d427a5a74760cc8f): Fixed logic for motorStatusPowerOn_
* Pull request [#2](https://github.com/epics-motor/motorNewport/pull/2): Eliminated compiler warnings

## __R1-0-1 (2019-08-08)__
R1-0-1 is a release based on the master branch.

### Changes since R1-0

#### Modifications to existing features
* Commit [cdd60e5](https://github.com/epics-motor/motorNewport/commit/cdd60e596f2e2855af6fd76c4320e7ed6102542f): Added build rule to allow *.req files to be installed when building against base 3.14

#### Bug fixes
* Commit [c9beba9](https://github.com/epics-motor/motorNewport/commit/c9beba9bbca2b55f4a068a445603fb3a9bf05660): Corrected a typo that prevented XPSTclScript.template from being installed

## __R1-0 (2019-04-18)__
R1-0 is a release based on the master branch.  

### Changes since motor-6-11

motorNewport is now a standalone module, as well as a submodule of [motor](https://github.com/epics-modules/motor)

#### New features
* motorNewport can be built outside of the motor directory
* motorNewport has a dedicated example IOC that can be built outside of motorNewport

#### Modifications to existing features
* Commit [808e26e](https://github.com/epics-motor/motorNewport/commit/808e26ef1a8418d8d07742454a6c311e77d5654a): HXP driver polling moved from axis to controller method to support controllers with newer firmware. Details can be found on the [motor issue #124](https://github.com/epics-modules/motor/issues/124).

#### Bug fixes
* None
