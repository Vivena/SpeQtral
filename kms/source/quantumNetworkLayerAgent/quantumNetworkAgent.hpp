#pragma once

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"


#include "dto/QNLAgentDTOs.hpp"
#include "keyManagmentAgent/keyStorage/keyPoolManager.hpp"
#include "keyManagmentAgent/keyStorage/keyPoolManagerDefaultConfig.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class KeySupplyAgent : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   * @param keyPoolManager - object for managing keys
   */
  KeySupplyAgent(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
                 OATPP_COMPONENT(std::shared_ptr<KeyPoolManager>, keyPoolManager))
    : oatpp::web::server::api::ApiController(objectMapper)
    , m_keyPoolMgr(keyPoolManager)
  {}

public:

  ENDPOINT("GET","/kmapi/versions", getVersion){
    
    return createDtoResponse(Status::CODE_200, dto);
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
  
  

private:
  OATPP_COMPONENT(std::shared_ptr<KeyPoolManager>, m_keyPoolMgr);

};

#include OATPP_CODEGEN_END(ApiController) 
