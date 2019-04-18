#!../../bin/linux-x86_64/newport

< envPaths

## Register all support components
dbLoadDatabase "../../dbd/newport.dbd"
newport_registerRecordDeviceDriver pdbbase

## motorUtil (allstop & alldone)
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=newport:")

#
#!< motor.cmd.hxp
#!< ESP300.cmd 
#!< MM3000.cmd
#!< MM4000.cmd
#!< MM4000Asyn.cmd
#!< PM500.cmd

iocInit

## motorUtil (allstop & alldone)
motorUtilInit("newport:")

# Boot complete

#
### Show some of the help messages
#
#!help AG_CONEXCreateController
# 
#!help AG_UCCreateController
#!help AG_UCCreateAxis
#
#!help ESP300Config
#!help ESP300Setup
#
#!help HXPCreateController
#
#!help SMC100CreateController
#
#!help XPSCreateController
#!help XPSCreateAxis
