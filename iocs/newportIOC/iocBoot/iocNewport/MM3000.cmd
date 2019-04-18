drvAsynSerialPortConfigure("serial1", "/dev/ttyS0", 0, 0, 0)

dbLoadTemplate("MM3000.substitutions")

# Newport MM3000 driver setup parameters: 
#     (1) maximum number of controllers in system
#     (2) motor task polling rate (min=1Hz,max=60Hz)
MM3000Setup(1, 10)

# Newport MM3000 driver configuration parameters: 
#     (1) controller# being configured
#     (2) ASYN port name
#     (3) address (GPIB only)
MM3000Config(0, "serial1")
#!var drvMM3000debug 4

# The MM3000 driver does not set end of string (EOS).
#  for RS232 serial,
#!asynOctetSetInputEos( "serial1",0,"\r")
#!asynOctetSetOutputEos("serial1",0,"\r")
