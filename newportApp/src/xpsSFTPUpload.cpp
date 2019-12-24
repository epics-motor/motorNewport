#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <epicsExport.h>
#include <xpsSFTPUpload.h>

// The following version requires libcurl.
#ifdef HAVE_LIBCURL
#include <sys/stat.h>
#include <curl/curl.h>

epicsShareFunc int xpsSFTPUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
                  std::string userName, std::string password, bool verbose) {
  int status = 0;
  CURL *curl = curl_easy_init();
  if (curl == 0) {
    fprintf(stderr, "curl_easy_init failed\n");
    return -1;
  }
  std::string url = "scp://" + IPAddress + "/" + trajectoryDirectory + "/" + fileName;
  status |= curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  status |= curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PASSWORD);
  std::string auth = userName + ":" + password;
  status |= curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());
  status |= curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
  status |= curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  status |= curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
  FILE *fd = fopen(fileName.c_str(), "rb"); /* open file to upload */
  struct stat fileStat;
  fstat(fileno(fd), &fileStat);
  status |= curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)fileStat.st_size);
  status |= curl_easy_setopt(curl, CURLOPT_READDATA, fd);
  status |= curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1);
  if (verbose) status |= curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
  status |= curl_easy_perform(curl);
  fclose(fd);
  curl_easy_cleanup(curl);
  if (status) {
    fprintf(stderr, "One or more curl_easy functions failed\n");
    return -1;
  }
  return 0;
}

#elif defined HAVE_LIBSSH2

#include <libssh2.h>
#include <libssh2_sftp.h>
#include <osiSock.h>

epicsShareFunc int xpsSFTPUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
                  std::string userName, std::string password, bool verbose) {
  struct sockaddr_in sockAddr;
  SOCKET sockFD = 0;
  LIBSSH2_SESSION *session = 0;
  LIBSSH2_SFTP_HANDLE *sftp_handle = 0;
  LIBSSH2_SFTP *sftp_session = 0;
  const char *fingerprint;
  int i;
  char mem[1024*100];
  FILE *local = 0;
  size_t nread;
  char *ptr;
  int rc;
  int one = 1;
  int status = -1;
  std::string remoteFile = trajectoryDirectory + fileName;

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
  setsockopt(sockFD, SOL_TCP, TCP_NODELAY, &one, sizeof(one));
 
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_port = htons(22);
  sockAddr.sin_addr.s_addr = inet_addr(IPAddress.c_str());
  if (connect(sockFD, (struct sockaddr*)(&sockAddr),
             sizeof(struct sockaddr_in)) != 0) {
    fprintf(stderr, "failed to connect to %s\n", IPAddress.c_str());
    goto done;
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
    goto done;
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
  if (libssh2_userauth_password(session, userName.c_str(), password.c_str())) {
    fprintf(stderr, "Authentication by password failed.\n");
    goto done;
  }

  local = fopen(fileName.c_str(), "rb");
  if (!local) {
    fprintf(stderr, "Can't open local file %s\n", fileName.c_str());
    goto done;
  }
 
  sftp_session = libssh2_sftp_init(session);
  if(!sftp_session) {
    fprintf(stderr, "Unable to init SFTP session\n");
    goto done;
  }
 
  /* Request a file via SFTP */
  sftp_handle = libssh2_sftp_open(sftp_session, remoteFile.c_str(),
                    LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
                    LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                    LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
 
  if(!sftp_handle) {
    fprintf(stderr, "Unable to open remote file %s with SFTP\n", remoteFile.c_str());
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
  if (session) {
    libssh2_session_disconnect(session, "Normal Shutdown");
    libssh2_session_free(session);
  }
  if (sockFD) {
    epicsSocketDestroy(sockFD);
  }
  return status;
}

#else
// If HAVE_LIBCURL is not YES then try to execute the curl system command
epicsShareFunc int xpsSFTPUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
                  std::string userName, std::string password, bool verbose) {
  std::string verboseOption = verbose ? " --verbose " : " --silent ";
  std::string command = "curl -k -u " + userName + ":" + password + " -T " + fileName + verboseOption + " scp://" + IPAddress + trajectoryDirectory + "/";
  if (verbose) printf("curl command=%s\n", command.c_str());
  int status = system(command.c_str());
  return status;
}

#endif
