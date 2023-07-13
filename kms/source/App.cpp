#include <iostream>
#include <vector>

#include "oatpp/network/Server.hpp"
#include "oatpp/core/base/CommandLineArguments.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp-swagger/Controller.hpp"
#include <yaml-cpp/yaml.h>


#include "keyManagmentAgent/keyStorage/keyPoolManager.hpp"
#include "quantumNetworkLayerAgent/quantumNetworkAgent.hpp"
#include "keySupplyAgent/keySupplyAgent.hpp"
#include "AppComponent.hpp"


// include your controllers here
// #include "./Controller1.hpp"
// #include "./Controller2.hpp"

void run(std::shared_ptr<oatpp::base::CommandLineArguments> args, std::shared_ptr<YAML::Node> config) {

  /* Register Components in scope of run() method */
  // AppComponent component(args, config);
  AppComponent component(args, config);

  /* Get KeyPoolManager component */
  auto keyPoolManager = std::make_shared<KeyPoolManager>(args, config);
  /* Get swaggerController component */
  auto KeySupplySwaggerController =
    oatpp::swagger::Controller::createShared(KeySupplyAgent().getEndpoints()); 

// TODO: refacto and put everything in the appropriate header

    /* Get ObjectMapper component for the keySupply*/  
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper);
  /* Get router component for the keySupply*/
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, KeySupplyhttpRouter, Qualifiers::KEYSUPPLY_SERVICE);
  /* Create MyController and add all of its endpoints to router */
  KeySupplyhttpRouter->addController(std::make_shared<KeySupplyAgent>(apiObjectMapper, keyPoolManager)); 
  KeySupplyhttpRouter->addController(KeySupplySwaggerController);
  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, KeySupplyserverConnectionHandler, Qualifiers::KEYSUPPLY_SERVICE);
  // /* Get connection provider component */
  // OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, KeySupplyProvider, Qualifiers::KEYSUPPLY_SERVICE);
  // /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  // oatpp::network::Server server(KeySupplyProvider, KeySupplyserverConnectionHandler);



  auto QuantumNetworkSwaggerController =
    oatpp::swagger::Controller::createShared(QuantumNetworkAgent().getEndpoints()); 

  /* Get router component for the QuantumNetwork*/
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, QuantumNetworkhttpRouter, Qualifiers::QUANTUMNETWORK_SERVICE);
  /* Create MyController and add all of its endpoints to router */
  QuantumNetworkhttpRouter->addController(std::make_shared<QuantumNetworkAgent>());
  QuantumNetworkhttpRouter->addController(QuantumNetworkSwaggerController);
  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, quantumNetworkserverConnectionHandler, Qualifiers::QUANTUMNETWORK_SERVICE);
  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, quantumNetworkProvider, Qualifiers::QUANTUMNETWORK_SERVICE);
  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server quantumNetworkServer(quantumNetworkProvider, quantumNetworkserverConnectionHandler);
  

  /* Run server */
  std::list<std::thread> acceptingThreads;

  // TODO: put in its own runner class
  acceptingThreads.push_back(std::thread([KeySupplyhttpRouter, KeySupplyserverConnectionHandler]{

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider, Qualifiers::KEYSUPPLY_SERVICE);
    oatpp::network::Server server(connectionProvider, KeySupplyserverConnectionHandler);
    OATPP_LOGI("facade", "server is listening on port '%s'", connectionProvider->getProperty("port").getData());
    server.run();

  }));
  
  // quantumNetworkServer.run();
  acceptingThreads.push_back(std::thread([QuantumNetworkhttpRouter, quantumNetworkserverConnectionHandler]{

    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider, Qualifiers::QUANTUMNETWORK_SERVICE);
    oatpp::network::Server server(connectionProvider, quantumNetworkserverConnectionHandler);
    OATPP_LOGI("facade", "server is listening on port '%s'", connectionProvider->getProperty("port").getData());
    server.run();

  }));


  for(auto& thread : acceptingThreads) {
    thread.join();
  }
  
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
