drvAsynSerialPortConfigure("serial4", "/dev/ttyS3", 0, 0, 0)

dbLoadTemplate("ESP300.substitutions")

# Newport ESP300 driver setup parameters: 
#     (1) maximum number of controllers in system
#     (2) motor task polling rate (min=1Hz,max=60Hz)
ESP300Setup(1, 10)

# Newport ESP300 driver configuration parameters: 
#     (1) controller# being configured,
#     (2) ASYN port name
#     (3) address (GPIB only)
ESP300Config(0, "serial4")
#!var drvESP300debug 4
