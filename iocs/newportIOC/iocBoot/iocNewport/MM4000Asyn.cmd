drvAsynSerialPortConfigure("serial2", "/dev/ttyS1", 0, 0, 0)

dbLoadTemplate("MM4000Asyn.substitutions")

# Newport MM4000/5/6 asyn motor driver setup parameter.
MM4000AsynSetup(1)   /* number of MM4000 controllers in system.  */

# Newport MM4000/5/6 asyn motor driver configure parameters.
#     (1) Controller number being configured
#     (2) ASYN port name
#     (3) ASYN address (GPIB only)
#     (4) Number of axes this controller supports
#     (5) Time to poll (msec) when an axis is in motion
#     (6) Time to poll (msec) when an axis is idle. 0 for no polling
MM4000AsynConfig(0, "serial2", 0, 1, 100, 250)

# Asyn-based Motor Record support
#   (1) Asyn port
#   (2) Driver name
#   (3) Controller index
#   (4) Max. number of axes
drvAsynMotorConfigure("MM4",   "motorMM4000", 0, 4)
