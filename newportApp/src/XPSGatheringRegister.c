#include <iocsh.h>
#include <epicsExport.h>

void xps_gathering(void);

static const iocshArg XPSGatheringArg0 = {"Interelement period", iocshArgInt};
static const iocshArg * const XPSGatheringArgs[1] = {&XPSGatheringArg0};
static const iocshFuncDef XPSGathering = {"XPSGathering", 1, XPSGatheringArgs};
static void XPSGatheringCallFunc(const iocshArgBuf *args)
{
    (void)args;
    xps_gathering();
}
static void XPSGatheringRegister(void)
{
    iocshRegister(&XPSGathering, XPSGatheringCallFunc);
}

epicsExportRegistrar(XPSGatheringRegister);


