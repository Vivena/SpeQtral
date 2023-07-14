#ifndef kms_keyRelay_KeyRelayController_hpp
#define kms_keyRelay_KeyRelayController_hpp

#include "../dto/keyRelayDTOs.hpp"
#include "../../../keyStoreManager/src/manager/keyPoolManager.hpp"
#include "../../../keyStoreManager/src/Constants.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace kms { namespace keyRelay { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen


/**
 * Sample Api Controller.
 */
class KeyRelayAgent : public oatpp::web::server::api::ApiController {

public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   * @param keyPoolManager - object for managing keys
   */
  KeyRelayAgent(OATPP_COMPONENT(std::shared_ptr<kms::keypool::manager::KeyPoolManager>, keyPoolManager),
                OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifiers::INTERFACE_KEYRELAY))
    : oatpp::web::server::api::ApiController(objectMapper)
    , m_keyPoolMgr(keyPoolManager)
  {}

public:

  ENDPOINT("GET","/kmapi/versions", getVersion){
    auto dtoOut = ACK_containers::createShared();
    return createDtoResponse(Status::CODE_200, dtoOut);
  }

  
  ENDPOINT("POST", "/kmapi/v1/ext_keys/{initiator_sae_id}/{target_sae_id}", transferKey,
          PATH(String, initiator_sae_id),
          PATH(String, target_sae_id),
          BODY_DTO(Object<TransferKeyDto>,  transferKeyDto))
  {
      auto dtoIn = transferKeyDto;
      auto dtoOut = ACK_containers::createShared();

      if (initiator_sae_id.get()) {
          OATPP_LOGD("Test", "initiator_sae_id='%s'", initiator_sae_id->c_str());
      } else {
        // TODO: create valid dtoOut for when we fail
          return createDtoResponse(Status::CODE_400, dtoOut);
      }
      if (target_sae_id.get()) {
          OATPP_LOGD("Test", "target_sae_id='%s'", target_sae_id->c_str());
          
      } else {
          // TODO: create valid dtoOut for when we fail
          return createDtoResponse(Status::CODE_400, dtoOut);
      }

      
      return createDtoResponse(Status::CODE_200, dtoOut);
  }

// private:
  std::shared_ptr<kms::keypool::manager::KeyPoolManager> m_keyPoolMgr;

};

#include OATPP_CODEGEN_END(ApiController) 

}}}

#endif /* kms_keyRelay_KeyRelayController_hpp */