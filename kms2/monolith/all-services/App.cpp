#include <iostream>
#include <yaml-cpp/yaml.h>

#include <oatpp/core/base/CommandLineArguments.hpp>

#include "monolith/all-services/util/Util.hpp"
#include "kms2/keySupplyInterface/src/AppComponent.hpp"
#include "kms2/keySupplyInterface/src/Runner.hpp"

void run(oatpp::base::CommandLineArguments args, YAML::Node config) {

    //TODO: clean the input
    uint16_t keySupplyPortStr = std::stoi(args.getNamedArgumentValue( (std::string("--keySupply-port")).c_str()));

    kms::keySupply::AppComponent keySupplyComponents(
        {"localhost", keySupplyPortStr},      // User Service
        {"user.virtualhost", 0}   // User Service Virtual Host
    );

    std::list<std::thread> acceptingThreads;

    kms::keySupply::Runner keySupplyRunner;
    keySupplyRunner.run(acceptingThreads);

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