#include "server.hpp"

/**
 * @brief Adds a controller configuration to the server.
 * 
 * @param config The controller configuration to add.
 */
void Server::addController(const std::shared_ptr<ControllerConfig>& config) {
    if (!config) {
        throw std::invalid_argument("Config is null");
    }
    auto router = std::make_shared<oatpp::web::server::HttpRouter>();
    config->getController()->addEndpointsToRouter(router);
    auto handler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    auto provider = oatpp::network::tcp::server::ConnectionProvider::createShared(config->createConnectionProviderConfig());
    auto server = std::make_shared<oatpp::network::Server>(provider, handler);
    m_controllerConfigs.push_back(config);
    m_servers.push_back(server);
}

/**
 * @brief Runs the server.
 */
void Server::run() {
    for (size_t i = 0; i < m_servers.size(); i++) {
        try {
            auto provider = oatpp::network::tcp::server::ConnectionProvider::createShared(m_controllerConfigs[i]->createConnectionProviderConfig());
            OATPP_LOGI("MyApp", "Server %d running on port %s...", i + 1, provider->getProperty("port").getData());
            m_servers[i]->run();
        }
        catch (const std::exception& e) {
            OATPP_LOGE("MyApp", "Exception occurred: %s", e.what());
            // Handle the exception as needed
        }
    }
}

