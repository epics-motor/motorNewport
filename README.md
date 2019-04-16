# motorNewport
EPICS motor drivers for the following [Newport](https://www.newport.com/) controllers:<br>
MM3000, MM4000/5/6, PM500, ESP300/301/100, XPSC8, HXP100-ELEC, AG_CONEX and AG_UC

motorNewport is a submodule of [motor](https://github.com/epics-modules/motor).  When motorNewport is built in the ``motor/modules`` directory, no manual configuration is needed.

motorNewport can also be built outside of motor by copying it's ``EXAMPLE_RELEASE.local`` file to ``RELEASE.local`` and defining the paths to ``MOTOR`` and itself.

motorNewport contains an example IOC that is built if ``CONFIG_SITE.local`` sets ``BUILD_IOCS = YES``.  The example IOC can be built outside of driver module.
