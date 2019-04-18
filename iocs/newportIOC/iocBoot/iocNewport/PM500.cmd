drvAsynSerialPortConfigure("serial3", "/dev/ttyS2", 0, 0, 0)

dbLoadTemplate("PM500.substitutions")

# Newport PM500 driver setup parameters:
#     (1) maximum number of controllers in system
#     (2) motor task polling rate (min=1Hz,max=60Hz)
PM500Setup(1, 10)

# Newport PM500 configuration parameters:
#     (1) controller# being configured,
#     (2) ASYN port name
#     (3) address (GPIB only)
PM500Config(0, "serial3")
#!var drvPM500debug 4
