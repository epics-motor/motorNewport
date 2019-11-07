#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <curl/curl.h>

#include <xpsCurlUpload.h>

int xpsCurlUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
                  std::string userName, std::string password) {
  int status = 0;
  CURL *curl = curl_easy_init();
  if (curl == 0) {
    fprintf(stderr, "curl_easy_init failed\n");
    return -1;
  }
  std::string url = "scp://" + IPAddress + "/" + trajectoryDirectory + "/" + fileName;
  status |= curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  std::string auth = userName + ":" + password;
  status |= curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());
  status |= curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
  FILE *fd = fopen(fileName.c_str(), "rb"); /* open file to upload */
  struct stat fileStat;
  fstat(fileno(fd), &fileStat);
  status |= curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)fileStat.st_size);
  status |= curl_easy_setopt(curl, CURLOPT_READDATA, fd);
//  status = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
  status |= curl_easy_perform(curl);
  fclose(fd);
  curl_easy_cleanup(curl);
  if (status) {
    fprintf(stderr, "One or more curl_easy functions failed\n");
    return -1;
  }
  return 0;
}