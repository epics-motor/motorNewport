/******[ includes ]**************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <libssh2.h>
#include <libssh2_sftp.h>

#include <osiSock.h>
#include <epicsExport.h>


#ifdef __cplusplus
extern "C" {
#endif

/******[ prototypes ]************************************************/
/* FTP commands */
epicsShareFunc int sftpConnect(char*, char*, char*, SOCKET*, LIBSSH2_SESSION **sslSession);
epicsShareFunc int sftpDisconnect(SOCKET sockFD, LIBSSH2_SESSION *sslSession);
epicsShareFunc int sftpStoreFile(SOCKET sockFD, LIBSSH2_SESSION *sslSession, char* localFile, char *remotePath);
epicsShareFunc int sftpReceiveFile(SOCKET sockFD, LIBSSH2_SESSION *sslSession, char *remotePath, char* localFile);

#ifdef __cplusplus
}
#endif
