drvAsynSerialPortConfigure("serial2", "/dev/ttyS1", 0, 0, 0)

dbLoadTemplate("MM4000.substitutions")

# Newport MM4000/5/6 driver setup parameters: 
#     (1) maximum number of controllers in system
#     (2) motor task polling rate (min=1Hz,max=60Hz)
MM4000Setup(1, 10)

# Newport MM4000/5/6 driver configuration parameters: 
#     (1) controller# being configured
#     (2) ASYN port name
#     (3) address (GPIB only)
MM4000Config(0, "serial2")
#!var drvMM4000debug 4

# The MM4000 driver does not set end of string (EOS).
#  for RS232 serial,
#!asynOctetSetInputEos( "serial2",0,"\r")
#!asynOctetSetOutputEos("serial2",0,"\r")
