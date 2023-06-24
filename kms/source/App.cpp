#include <iostream>
#include <vector>

#include "oatpp/network/Server.hpp"
#include "oatpp/core/base/CommandLineArguments.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp-swagger/Controller.hpp"
#include <yaml-cpp/yaml.h>

#include "keySupplyAgent/keySupplyAgent.hpp"
#include "AppComponent.hpp"


// include your controllers here
// #include "./Controller1.hpp"
// #include "./Controller2.hpp"

void run(std::shared_ptr<oatpp::base::CommandLineArguments> args, std::shared_ptr<YAML::Node> config) {

  /* Register Components in scope of run() method */
  // AppComponent component(args, config);
  AppComponent component(args, config);


  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
  /* Create MyController and add all of its endpoints to router */
  router->addController(std::make_shared<KeySupplyAgent>());

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());
  
  auto swaggerController =
    oatpp::swagger::Controller::createShared(KeySupplyAgent().getEndpoints());  
  router->addController(swaggerController);

  /* Run server */
  server.run();
  
}

int main(int argc, const char * argv[]) {

  oatpp::base::Environment::init();
  
  std::shared_ptr<YAML::Node> config;
  auto args= std::make_shared<oatpp::base::CommandLineArguments>(argc, argv);
 
  auto yamlConfigFile = (*args).getNamedArgumentValue("--yaml");
  if (yamlConfigFile != nullptr && *yamlConfigFile != '\0') {
    config = config = std::make_shared<YAML::Node>(YAML::LoadFile(yamlConfigFile));
  }
  run(args, config);
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();
  
  return 0;
}
