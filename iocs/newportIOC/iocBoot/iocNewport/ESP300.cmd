#drvAsynSerialPortConfigure("serial4", "/dev/ttyS0", 0, 0, 0)
drvAsynSerialPortConfigure("serial4", "/dev/ttyUSB0", 0, 0, 0)

# Configure the serial port
asynSetOption("serial4",0,"baud","19200")
asynSetOption("serial4",0,"bits","8")
asynSetOption("serial4",0,"stop","1")
asynSetOption("serial4",0,"parity","none")
asynSetOption("serial4",0,"clocal","Y")
asynSetOption("serial4",0,"crtscts","N")
asynOctetSetOutputEos("serial4",0,"\r")
asynOctetSetInputEos("serial4",0,"\r\n")

# Uncomment following lines to show debug messages
#asynSetTraceIOMask(serial4, 0, ESCAPE)
#asynSetTraceMask(serial4, 0, ERROR|DRIVER|FLOW)

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
