#ifndef util_hpp
#define util_hpp

#include "kms2/monolith/all-services/util/serverDefaultConfig.hpp"

#define PORT_SUFFIX "_PORT"

#define STRINGIFY_ENUM(x) std::string(#x)

#define STRINGIFY_LOWER_CASE(x) []{ \
    std::string str(#x);     \
    std::transform(str.begin(), str.end(), str.begin(), ::tolower); \
    return str;              \
}();

enum ServicesList{
    KEYSUPPLYAGENT = 1,
    AUTHSERVICE,
    RESTE,
};

const int defaultPortNumber(int serviceType);

std::string servicesListStrings(int serviceType);

uint16_t getPort(oatpp::base::CommandLineArguments args, YAML::Node config, int serviceType);

#endif 