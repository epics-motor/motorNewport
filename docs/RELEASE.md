# motorNewport Releases

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
