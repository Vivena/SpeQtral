#include <iostream>
#include <yaml-cpp/yaml.h>

#include <oatpp/core/base/CommandLineArguments.hpp>
#include "oatpp/core/macro/component.hpp"

#include "../../util/src/Util.hpp"
#include "../../keyStoreManager/src/manager/keyPoolManager.hpp"
#include "../../keyStore/src/keyStorage/keyPool.hpp"

#include "../../keySupplyInterface/src/AppComponent.hpp"
#include "../../keySupplyInterface/src/Runner.hpp"

#include "../../keyRelayInterface/src/AppComponent.hpp"
#include "../../keyRelayInterface/src/Runner.hpp"

void run(oatpp::base::CommandLineArguments args, YAML::Node config) {

    uint16_t keySupplyPortStr = getPort( args, config, KEYSUPPLYAGENT);
    uint16_t keyRelayPortStr = getPort( args, config, KEYRELAYAGENT);

    std::shared_ptr<kms::keypool::manager::KeyPoolManager> keyPoolManager = std::make_shared<kms::keypool::manager::KeyPoolManager>();

    kms::keySupply::AppComponent keySupplyComponents(
      {"localhost", keySupplyPortStr},      // User Service
      {"keySupply.virtualhost", 0}   // User Service Virtual Host
    );

    kms::keyRelay::AppComponent keyRelayComponents(
      {"localhost", keyRelayPortStr},      // User Service
      {"keyRelay.virtualhost", 0}   // User Service Virtual Host
    );

    std::list<std::thread> acceptingThreads;
    
    auto keySupplyRunner = kms::keySupply::Runner(keyPoolManager);
    keySupplyRunner.run(acceptingThreads);

    auto keyRelayRunner = kms::keyRelay::Runner(keyPoolManager);
    keyRelayRunner.run(acceptingThreads);

    for(auto& thread : acceptingThreads) {
      thread.join();
    }

}


int main(int argc, const char * argv[]) {

  oatpp::base::Environment::init();
  
  YAML::Node config;
  auto args= oatpp::base::CommandLineArguments(argc, argv);
 
  auto yamlConfigFile = args.getNamedArgumentValue("--yaml");
  if (yamlConfigFile != nullptr && *yamlConfigFile != '\0') {
    config = YAML::Node(YAML::LoadFile(yamlConfigFile));
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