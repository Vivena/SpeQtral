#include "Runner.hpp"

#include "AppComponent.hpp"

#include "controller/KeySupplyInterfaceController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

namespace kms { namespace keySupply {

    Runner::Runner() {
    
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifiers::INTERFACE_KEYSUPPLY);

        oatpp::web::server::api::Endpoints docEndpoints;

        /* Add BookController */
        docEndpoints.append(router->addController(std::make_shared<controller::BookController>())->getEndpoints());

        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo, Qualifiers::INTERFACE_KEYSUPPLY);
        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources, Qualifiers::INTERFACE_KEYSUPPLY);

        router->addController(oatpp::swagger::Controller::createShared(docEndpoints, documentInfo, resources));

    }

}}