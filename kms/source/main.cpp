#include <vector>
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/app/Controller.hpp"

#include "./server.hpp"
// include your controllers here
#include "./Controller1.hpp"
#include "./Controller2.hpp"


/**
 * @brief The main function of the program.
 * 
 * @return int Exit code.
 */
int main() {
  // Parse controller configuration data from JSON file
  auto configs = parseControllerConfigsFromYamlFile("/path/to/config.yaml");

  // Create server instance
  MyServer server;

  // Add each controller to the server with its corresponding configuration
  for (auto& config : configs) {
    server.addController(config);
  }

  // Start the server
  server.run();

  return 0;
}
