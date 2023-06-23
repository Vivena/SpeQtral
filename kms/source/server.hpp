#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include "./configService/controllerConfig.hpp"

/**
 * @brief Encapsulates the creation and management of the HTTP server.
 */
class Server {

public:
  /**
   * @brief Adds a controller configuration to the server.
   * 
   * @param config The controller configuration to add.
   */
  void addController(const std::shared_ptr<ControllerConfig>& config);

  /**
   * @brief Runs the server.
   */
  void run();

  private:
  std::vector<std::shared_ptr<ControllerConfig>> m_controllerConfigs;
  std::vector<std::shared_ptr<oatpp::network::Server>> m_servers;
};
