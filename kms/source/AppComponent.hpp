#ifndef AppComponent_hpp
#define AppComponent_hpp

#include <iostream>
#include <unistd.h>



#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/base/CommandLineArguments.hpp"
#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include <yaml-cpp/yaml.h>

#define PORT_SUFFIX "_PORT"

// TODO: put those macro in it correct place
#define KEYSUPPLYAGENT_DEFAULT_PORT 9060
#define AUTHSERVICE_DEFAULT_PORT 9992
#define RESTE_DEFAULT_PORT 8761


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


/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {

public:
  AppComponent(std::shared_ptr<oatpp::base::CommandLineArguments> cmdArgs, std::shared_ptr<YAML::Node> config)
  : m_args(cmdArgs)
  , m_config(config)
  {}

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, KeySupplyProvider)([this]() -> std::shared_ptr<oatpp::network::ServerConnectionProvider> {
    
    auto port = getPort(KEYSUPPLYAGENT);

    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", port, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, KeySupplyhttpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, KeySupplyserverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, KeySupplyapiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  OATPP_CREATE_COMPONENT(
  std::shared_ptr<oatpp::swagger::DocumentInfo>, 
  swaggerDocumentInfo
  )([this] {
  
    oatpp::swagger::DocumentInfo::Builder builder;  builder
    .setTitle("My Demo Service with Swagger-UI")
    .setDescription("C++/oat++ Web Service with Swagger-UI")
    .setVersion("1.0")
    .setContactName("Mr. Developer")
    .setContactUrl("https://oatpp.io/")
    .setLicenseName("Apache License, Version 2.0")
    .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
    .addServer("http://localhost:" + std::to_string(getPort(KEYSUPPLYAGENT)), "server on localhost");   return builder.build();
  }());
  
  /**
   * This block of code defines a shared pointer to an oatpp::swagger::Resources object, named "swaggerResources".
   * This object loads the swagger resources from the OATPP_SWAGGER_RES_PATH with the loadResources() method.
   */
  OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::swagger::Resources>, 
      swaggerResources
    )([] {  return oatpp::swagger::Resources::loadResources(
        OATPP_SWAGGER_RES_PATH
      );}());
  
  /**
   * This function sets the `isINIT` flag to true.
   */
  void setINIT(){
    isINIT = true;
  }
  

private:
  /**
   * The following functions handle getting default port numbers for the specified service types.
   *
   * @param serviceType The type of service to get the port number for.
   * @return Returns a string representing the specified enum as a lowercased string.
   */
  const std::string servicesListStrings(int serviceType) {
    switch (serviceType) {
      case KEYSUPPLYAGENT: return STRINGIFY_ENUM(KEYSUPPLYAGENT);
      case AUTHSERVICE: return STRINGIFY_ENUM(AUTHSERVICE);
      case RESTE: return STRINGIFY_ENUM(RESTE);
      default: return "";
    }
  };

  /**
   * Given a `serviceType`, this function returns the default port number for that type of service.
   *
   * @param serviceType The type of service to get the port number for.
   * @return Returns an integer representing the default port for the specified serviceType.
   */
  const int defaultPortNumber(int serviceType) {
    switch (serviceType) {
        case KEYSUPPLYAGENT: return KEYSUPPLYAGENT_DEFAULT_PORT;
        case AUTHSERVICE: return AUTHSERVICE_DEFAULT_PORT;
        case RESTE: return RESTE_DEFAULT_PORT;
        default: return -1; // Handle unexpected input (e.g. invalid enum value)
    }
  }

  /**
   * Given a `serviceType`, this function returns the corresponding service type as a lowercased string.
   *
   * @param serviceType The type of service to get the string representation for.
   * @return Returns a lowercased string representing the specified serviceType.
   */
  const std::string getServicesStrings(int serviceType) {
    switch (serviceType) {
        case KEYSUPPLYAGENT: return STRINGIFY_LOWER_CASE(KEYSUPPLYAGENT);
      case AUTHSERVICE: return STRINGIFY_LOWER_CASE(AUTHSERVICE);
      case RESTE: return STRINGIFY_LOWER_CASE(RESTE);
      default: return "";
    }
  }

  /**
   * This inline function returns the port number for a specified `serviceType`.
   * It tries to get the port from the environment variable and then from the command-line argument.
   * If neither are found, it uses the default port.
   *
   * @param serviceType The type of service to get the port number for.
   * @return Returns a v_uint16 representing the port number to use.
   */
  inline v_uint16 getPort(int serviceType){
    v_uint16 port;
    const char* portStr = nullptr;

      // Try to get port from environment variable
      const std::string portSuffix = PORT_SUFFIX;
      const char* envPortStr = std::getenv((servicesListStrings(serviceType) + portSuffix).c_str());
      if(envPortStr != nullptr) {
        portStr = envPortStr;
      }
      
      // Try to get port from command-line argument
      else {
        auto& args = *m_args;
        auto& config = *m_config;
        portStr = args.getNamedArgumentValue( (std::string("--") + getServicesStrings(serviceType) +  std::string("-port")).c_str());
        // If not found, try to get it from YAML configuration file
        if(portStr == nullptr && config != nullptr && config[getServicesStrings(serviceType)]["port"]) {
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
  
private:
  v_uint16 m_port;
  std::shared_ptr<oatpp::base::CommandLineArguments> m_args;
  std::shared_ptr<YAML::Node> m_config;
  bool isINIT = false;
};

#endif /* AppComponent_hpp */