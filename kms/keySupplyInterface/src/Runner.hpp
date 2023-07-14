
#ifndef kms_keySupply_Runner_hpp
#define kms_keySupply_Runner_hpp

#include "AppComponent.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "../../keyStoreManager/src/manager/keyPoolManager.hpp"

#include "controller/KeySupplyInterfaceController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

#include <list>
#include <thread>

namespace kms { namespace keySupply {

class Runner {

public:
  Runner(std::shared_ptr<kms::keypool::manager::KeyPoolManager> keyPoolManager){

        // OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper, , Qualifiers::INTERFACE_KEYSUPPLY);
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifiers::INTERFACE_KEYSUPPLY);

        oatpp::web::server::api::Endpoints docEndpoints;

        /* Add KeySupplyController */
        docEndpoints.append(router->addController(std::make_shared<kms::keySupply::controller::KeySupplyAgent>(keyPoolManager))->getEndpoints());

        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo, Qualifiers::INTERFACE_KEYSUPPLY);
        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources, Qualifiers::INTERFACE_KEYSUPPLY);

        router->addController(oatpp::swagger::Controller::createShared(docEndpoints, documentInfo, resources));
    }

  void run(std::list<std::thread>& acceptingThreads);

};

}}

#endif // kms_keySupply_Runner_hpp