#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/app/Controller.hpp"


/**
 * @brief Encapsulates the creation and management of the HTTP server.
 */
class MyServer {
private:
  std::vector<std::shared_ptr<ControllerConfig>> m_controllerConfigs;
  std::vector<std::shared_ptr<oatpp::network::Server>> m_servers;

public:
  /**
   * @brief Adds a controller configuration to the server.
   * 
   * @param config The controller configuration to add.
   */
  void addController(const std::shared_ptr<ControllerConfig>& config) {
    auto router = std::make_shared<oatpp::web::server::HttpRouter>();
    config->getController()->addEndpointsToRouter(router);
    auto handler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    auto provider = oatpp::network::tcp::server::ConnectionProvider::createShared(config->createConnectionProviderConfig());
    auto server = std::make_shared<oatpp::network::Server>({ {provider, handler} });
    m_controllerConfigs.push_back(config);
    m_servers.push_back(server);
  }

  /**
   * @brief Runs the server.
   */
  void run() {
    for (size_t i = 0; i < m_servers.size(); i++) {
      auto provider = oatpp::network::tcp::server::ConnectionProvider::createShared(m_controllerConfigs[i]->createConnectionProviderConfig());
      OATPP_LOGI("MyApp", "Server %d running on port %s...", i + 1, provider->getProperty("port").getData());
      m_servers[i]->runAsync();
    }
  }
};

/**
 * @brief Parses a JSON file containing controller configuration data.
 * 
 * @param filename The path to the JSON file.
 * @return std::vector<std::shared_ptr<ControllerConfig>> A vector of ControllerConfig instances.
 */
std::vector<std::shared_ptr<ControllerConfig>> parseControllerConfigsFromJsonFile(const oatpp::String& filename) {
  auto json = oatpp::parser::json::JsonParser::parseFromFile(filename);
  std::vector<std::shared_ptr<ControllerConfig>> configs;
  for (auto& controllerObj : json->getAs<oatpp::Vector<oatpp::Object<oatpp::data::mapping::type::UnorderedFields<String, oatpp::BaseObject>>>>()) {
    auto config = ControllerConfig::parseFromJson(controllerObj);
    configs.push_back(config);
  }
  return configs;
}