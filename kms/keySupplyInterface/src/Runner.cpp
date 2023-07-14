#include "Runner.hpp"

#include "AppComponent.hpp"

#include "controller/KeySupplyInterfaceController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

namespace kms { namespace keySupply {    

    void Runner::run(std::list<std::thread>& acceptingThreads) {

        /* Get router component */
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifiers::INTERFACE_KEYSUPPLY);

        /* Create connection handler */
        auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

        acceptingThreads.push_back(std::thread([router, connectionHandler]{

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider, Qualifiers::INTERFACE_KEYSUPPLY);
        oatpp::network::Server server(connectionProvider, connectionHandler);
        OATPP_LOGI("keySupply-Interface", "server is listening on port '%s'", connectionProvider->getProperty("port").getData());
        server.run();

        }));

        acceptingThreads.push_back(std::thread([router, connectionHandler]{

        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider, Qualifiers::INTERFACE_KEYSUPPLY_VH);
        oatpp::network::Server server(connectionProvider, connectionHandler);
        OATPP_LOGI("keySupply-Interface", "server is listening on virtual interface '%s'", connectionProvider->getProperty("host").getData());
        server.run();

        }));
    }
}}