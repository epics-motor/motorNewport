#include <stdio.h>
#include <asynDriver.h>
#include <xpsCurlUpload.h>

int main(int argc, char *argv[])
{
  bool curlVerbose = true;
  asynUser *pasynUser = pasynManager->createAsynUser(0, 0);
  asynPrint(pasynUser, ASYN_TRACE_ERROR, "Calling xpsCurlUpload\n");
  int status = xpsCurlUpload("newport-xps16", "/Admin/Public/Trajectories", "TrajectoryScan.trj", 
                             "Administrator", "Administrator", curlVerbose);
  asynPrint(pasynUser, ASYN_TRACE_ERROR, "xpsCurlUpload returned status=%d\n", status);
  return 0;
}