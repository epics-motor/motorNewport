#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <xpsCurlUpload.h>

// The following version requires libcurl.
#ifdef HAVE_LIBCURL
#include <sys/stat.h>
#include <curl/curl.h>

int xpsCurlUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
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
  FILE *fd = fopen(fileName.c_str(), "rb"); /* open file to upload */
  struct stat fileStat;
  fstat(fileno(fd), &fileStat);
  status |= curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)fileStat.st_size);
  status |= curl_easy_setopt(curl, CURLOPT_READDATA, fd);
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
#else
// If HAVE_LIBCURL is not YES then try to execute the curl system command
int xpsCurlUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
                  std::string userName, std::string password, bool verbose) {
  std::string verboseOption = verbose ? " --verbose " : " --silent ";
  std::string command = "curl -k -u " + userName + ":" + password + " -T " + fileName + verboseOption + " scp://" + IPAddress + trajectoryDirectory + "/";
  if (verbose) printf("curl command=%s\n", command.c_str());
  int status = system(command.c_str());
  return status;
}

#endif
