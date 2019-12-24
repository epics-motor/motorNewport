#include <string>

epicsShareFunc int xpsSFTPUpload(std::string IPAddress, std::string trajectoryDirectory, std::string fileName, 
                                 std::string userName, std::string password, bool verbose);
