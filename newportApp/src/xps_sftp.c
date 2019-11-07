/* Simple functions to do SFTP from within C/C++ program
   Code based on libssh2.or/example/sftp_write.html 

  Mark Rivers
  University of Chicago
  November 5, 2019
*/
 
#include "xps_sftp.h"

/******[ sftpConnect ]************************************************/
epicsShareFunc int sftpConnect (char* ip, char* login, char* password, SOCKET* socketFD, LIBSSH2_SESSION **sslSession)
{
  struct sockaddr_in sockAddr;
  SOCKET sockFD;
  LIBSSH2_SESSION *session;
  const char *fingerprint;
  int rc;
  int i;
  
  rc = libssh2_init(0);
  if (rc != 0) {
    fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
    return -1;
  }
 
  /*
   * The application code is responsible for creating the socket
   * and establishing the connection
   */ 
  sockFD = epicsSocketCreate(AF_INET, SOCK_STREAM, 0);
 
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_port = htons(22);
  sockAddr.sin_addr.s_addr = inet_addr(ip);
  if (connect(sockFD, (struct sockaddr*)(&sockAddr),
             sizeof(struct sockaddr_in)) != 0) {
    fprintf(stderr, "failed to connect!\n");
    return -1;
  }
 
  /* Create a session instance */ 
  session = libssh2_session_init();

  /* Since we have set non-blocking, tell libssh2 we are blocking */ 
  libssh2_session_set_blocking(session, 1);

 
  /* ... start it up. This will trade welcome banners, exchange keys,
   * and setup crypto, compression, and MAC layers */ 
  rc = libssh2_session_handshake(session, sockFD);

  if (rc) {
    fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
    return -1;
  }
 
  /* At this point we haven't yet authenticated.  The first thing to do
   * is check the hostkey's fingerprint against our known hosts Your app
   * may have it hard coded, may go to a file, may present it to the
   * user, that's your call
   */ 
  fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

  fprintf(stderr, "Fingerprint: ");
  for (i = 0; i < 20; i++) {
    fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
  }
  fprintf(stderr, "\n");
 
  /* Authenticate via password */ 
  if (libssh2_userauth_password(session, login, password)) {
    fprintf(stderr, "Authentication by password failed.\n");
    goto shutdown;
  }

  *socketFD = sockFD;
  *sslSession = session;
  return 0;
  
  shutdown:
 
  sftpDisconnect(sockFD, session);
 
  return -1;
}


/******[ sftpDisconnect ]*********************************************/
epicsShareFunc int sftpDisconnect (SOCKET socketFD, LIBSSH2_SESSION *session)
{
  libssh2_session_disconnect(session, "Normal Shutdown");
  libssh2_session_free(session);

  epicsSocketDestroy(socketFD);
  return 0;

}

/******[ sftpRetrieveFile ]*******************************************/
epicsShareFunc int sftpRetrieveFile(SOCKET socketFD, LIBSSH2_SESSION *session, char *remotePath, char *localFile)
{
  LIBSSH2_SFTP *sftp_session = 0;
  LIBSSH2_SFTP_HANDLE *sftp_handle = 0;
  int rc;
  FILE *local = 0;
  int status = -1;

  local = fopen(localFile, "wb");
  if(!local) {
    fprintf(stderr, "Can't open local file %s\n", localFile);
    goto done;
  }
 
  sftp_session = libssh2_sftp_init(session);
  if(!sftp_session) {
    fprintf(stderr, "Unable to init SFTP session\n");
    goto done;
  }
 
  /* Request a file via SFTP */ 
  sftp_handle = libssh2_sftp_open(sftp_session, remotePath, LIBSSH2_FXF_READ, 0);
  if(!sftp_handle) {
    fprintf(stderr, "Unable to open file with SFTP: %ld\n",
            libssh2_sftp_last_error(sftp_session));
    goto done;
  }
  do {
    char mem[1024];
    /* loop until we fail */ 
    rc = libssh2_sftp_read(sftp_handle, mem, sizeof(mem));
    if(rc > 0) {
      fwrite(mem, 1, rc, local);
    }
    else {
      break;
    }
  } while(1);
  status = 0;

  done:
  if (local) fclose(local);
  if (sftp_handle) libssh2_sftp_close(sftp_handle);
  if (sftp_session) libssh2_sftp_shutdown(sftp_session);
  return status;
}


/******[ sftpStoreFile ]**********************************************/
epicsShareFunc int sftpStoreFile(SOCKET socketFD, LIBSSH2_SESSION *session, char *localFile, char *remotePath)
{
  LIBSSH2_SFTP *sftp_session = 0;
  LIBSSH2_SFTP_HANDLE *sftp_handle = 0;
  char mem[1024*100];
  FILE *local = 0;
  size_t nread;
  char *ptr;
  int rc;
  int status = -1;

  local = fopen(localFile, "rb");
  if(!local) {
    fprintf(stderr, "Can't open local file %s\n", localFile);
    goto done;
  }
 
  sftp_session = libssh2_sftp_init(session);
  if(!sftp_session) {
    fprintf(stderr, "Unable to init SFTP session\n");
    goto done;
  }
 
  /* Request a file via SFTP */ 
  sftp_handle = libssh2_sftp_open(sftp_session, remotePath,
                    LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
                    LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                    LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
 
  if(!sftp_handle) {
    fprintf(stderr, "Unable to open file %s with SFTP\n", remotePath);
    goto done;
  }

  do {
    nread = fread(mem, 1, sizeof(mem), local);
    if(nread <= 0) {
        /* end of file */ 
        break;
    }
    ptr = mem;
    do {
      /* write data in a loop until we block */ 
      rc = libssh2_sftp_write(sftp_handle, ptr, nread);
       if(rc < 0)
          break;
      ptr += rc;
      nread -= rc;
    } while(nread);
 
  } while(rc > 0);
  status = 0;
 
  done:
  if (local) fclose(local);
  if (sftp_handle) libssh2_sftp_close(sftp_handle);
  if (sftp_session) libssh2_sftp_shutdown(sftp_session);
  return status;
}
