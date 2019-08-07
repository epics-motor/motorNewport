# motorNewport Releases

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
