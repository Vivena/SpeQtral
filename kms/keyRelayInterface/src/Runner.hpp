
#ifndef kms_keyRelay_Runner_hpp
#define kms_keyRelay_Runner_hpp

#include "AppComponent.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "../../keyStoreManager/src/manager/keyPoolManager.hpp"

#include "controller/KeyRelayInterfaceController.hpp"

#include "oatpp-swagger/Controller.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

#include <list>
#include <thread>

namespace kms { namespace keyRelay {

class Runner {

public:
  Runner(std::shared_ptr<kms::keypool::manager::KeyPoolManager> keyPoolManager){

        // OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper, , Qualifiers::INTERFACE_KEYRELAY);
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router, Qualifiers::INTERFACE_KEYRELAY);

        oatpp::web::server::api::Endpoints docEndpoints;

        /* Add KeyRelayController */
        docEndpoints.append(router->addController(std::make_shared<kms::keyRelay::controller::KeyRelayAgent>(keyPoolManager))->getEndpoints());

        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, documentInfo, Qualifiers::INTERFACE_KEYRELAY);
        OATPP_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, resources, Qualifiers::INTERFACE_KEYRELAY);

        router->addController(oatpp::swagger::Controller::createShared(docEndpoints, documentInfo, resources));
    }

  void run(std::list<std::thread>& acceptingThreads);

};

}}

#endif // kms_keyRelay_Runner_hpp