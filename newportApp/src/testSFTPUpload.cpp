#include <stdio.h>
#include <epicsTime.h>
#include <epicsExport.h>
#include <xpsSFTPUpload.h>

int main(int argc, char *argv[])
{
  bool sftpVerbose = true;
  epicsTimeStamp startTime, endTime;
  epicsTimeGetCurrent(&startTime);
  printf("Calling xpsSFTPUpload\n");
  int status = xpsSFTPUpload("164.54.160.71", "/Admin/Public/Trajectories", "TrajectoryScan.trj", 
                             "Administrator", "Administrator", sftpVerbose);
  epicsTimeGetCurrent(&endTime);
  double elapsedTime = epicsTimeDiffInSeconds(&endTime, &startTime);
  printf("xpsSFTPUpload returned status=%d, elapsed time=%f\n", status, elapsedTime);
  return 0;
}

