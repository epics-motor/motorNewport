#!../../bin/linux-x86_64/newport

## You may have to change newport to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/newport.dbd"
newport_registerRecordDeviceDriver pdbbase

## motorUtil (allstop & alldone)
dbLoadRecords("$(MOTOR)/db/motorUtil.db", "P=newport:")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## motorUtil (allstop & alldone)
motorUtilInit("newport:")

# Boot complete
