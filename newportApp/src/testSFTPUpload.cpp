#include <stdio.h>
#include <asynDriver.h>
#include <xpsSFTPUpload.h>

int main(int argc, char *argv[])
{
  bool sftpVerbose = true;
  asynUser *pasynUser = pasynManager->createAsynUser(0, 0);
  asynPrint(pasynUser, ASYN_TRACE_ERROR, "Calling xpsSFTPUpload\n");
  int status = xpsSFTPUpload("164.54.160.71", "/Admin/Public/Trajectories", "TrajectoryScan.trj", 
                             "Administrator", "Administrator", sftpVerbose);
  asynPrint(pasynUser, ASYN_TRACE_ERROR, "xpsSFTPUpload returned status=%d\n", status);
  return 0;
}