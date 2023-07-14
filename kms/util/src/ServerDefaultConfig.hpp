#ifndef Server_Default_Config_hpp
#define Server_Default_Config_hpp

#include <stdio.h>

#define SUPPORTED_VERSIONS() \
  list("v1")
#define VERSION "3.0"
#define TITLE "Demo QKD-KMS"
#define DESCRIPTION "C++/oat++ QKD-KMS with Swagger-UI"
#define CONTACT_NAME "Edward Guyot"
#define CONTACT_URL "edward.guyot@gmail.com"

#define KEYSUPPLYAGENT_DEFAULT_PORT 9060
#define KEYRELAYAGENT_DEFAULT_PORT 9393
// #define SYNC_DEFAULT_PORT 12345


#endif