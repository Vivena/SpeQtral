#include <unistd.h>
#include <yaml-cpp/yaml.h>
#include <oatpp/core/base/CommandLineArguments.hpp>

#include "src/Util.hpp"


const int defaultPortNumber(int serviceType) {
    switch (serviceType) {
        case KEYSUPPLYAGENT: return KEYSUPPLYAGENT_DEFAULT_PORT;
        // case AUTHSERVICE: return AUTHSERVICE_DEFAULT_PORT;
        // case RESTE: return RESTE_DEFAULT_PORT;
        default: return -1; // Handle unexpected input (e.g. invalid enum value)
    }
}

std::string servicesListStrings(int serviceType) {
    switch (serviceType) {
      case KEYSUPPLYAGENT: return STRINGIFY_ENUM(KEYSUPPLYAGENT);
      case AUTHSERVICE: return STRINGIFY_ENUM(AUTHSERVICE);
      case RESTE: return STRINGIFY_ENUM(RESTE);
      default: return "";
    }
}

const std::string getServicesStrings(int serviceType) {
    switch (serviceType) {
        case KEYSUPPLYAGENT: return STRINGIFY_LOWER_CASE(KEYSUPPLYAGENT);
        case AUTHSERVICE: return STRINGIFY_LOWER_CASE(AUTHSERVICE);
        case RESTE: return STRINGIFY_LOWER_CASE(RESTE);
        default: return "";
    }
}

uint16_t getPort(oatpp::base::CommandLineArguments args, YAML::Node config, int serviceType){
    uint16_t port;

    const char* portStr = nullptr;

      // Try to get port from environment variable
    const std::string portSuffix = PORT_SUFFIX;
    const char* envPortStr = std::getenv((servicesListStrings(serviceType) + portSuffix).c_str());
    if(envPortStr != nullptr) {
        portStr = envPortStr;
    }
      
      // Try to get port from command-line argument
    else {
        portStr = args.getNamedArgumentValue( (std::string("--") + getServicesStrings(serviceType) +  std::string("-port")).c_str());
        // If not found, try to get it from YAML configuration file
        if(portStr == nullptr && !config.IsNull() && config[getServicesStrings(serviceType)]["port"]) {
            const auto& portProp = config[getServicesStrings(serviceType)]["port"];
            if(portProp.IsScalar()) {
                const auto& portStrValue = portProp.as<std::string>();
                if(!portStrValue.empty()) {
                    portStr = portStrValue.c_str();
                }
            }
        }
    }

      // Use default port if none specified
    if(portStr != nullptr) {
        port = atoi(portStr);
    } else {
        port = defaultPortNumber(serviceType);
    }
      return port;
}