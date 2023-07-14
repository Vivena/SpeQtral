#ifndef kms_keySupply_KeySupplyController_hpp
#define kms_keySupply_KeySupplyController_hpp

#include "../dto/keySupplyDTOs.hpp"
#include "../../../keyStoreManager/src/manager/keyPoolManager.hpp"
#include "../../../keyStoreManager/src/Constants.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

namespace kms { namespace keySupply { namespace controller {

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
  KeySupplyAgent(OATPP_COMPONENT(std::shared_ptr<kms::keypool::manager::KeyPoolManager>, keyPoolManager),
                OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper, Qualifiers::INTERFACE_KEYSUPPLY))
    : oatpp::web::server::api::ApiController(objectMapper)
    , m_keyPoolMgr(keyPoolManager)
  {}

public:

  ENDPOINT("GET","/api/v1/keys/{slave_SAE_ID}/status", getStatus, PATH(String, slave_SAE_ID)){
    auto dto = kms::keySupply::dto::SendStatusDto::createShared();
    OATPP_LOGD("Test", "slave_SAE_ID='%s'", slave_SAE_ID->c_str());
    return createDtoResponse(Status::CODE_200, dto);
  }

  
  ENDPOINT("POST", "/api/v1/keys/{slave_SAE_ID}/enc_keys", getKey,
          PATH(String, slave_SAE_ID),
          BODY_DTO(Object<kms::keySupply::dto::GetKeyDto>, getKeyDto))
  {
      auto dtoIn = getKeyDto;
      auto dtoOut = kms::keySupply::dto::SendKeyDto::createShared();

      if (slave_SAE_ID.get()) {
          OATPP_LOGD("Test", "slave_SAE_ID='%s'", slave_SAE_ID->c_str());
          auto k = m_keyPoolMgr->newKey(slave_SAE_ID);
          // TODO: add a check on the slave_SAE_ID
      } else {
          return createDtoResponse(Status::CODE_400, dtoOut);
      }

      if (dtoIn->number) {
        OATPP_LOGD("Test", "number='%d'", *dtoIn->number);
      } else {
        dtoIn->number = 1;
        OATPP_LOGD("Test", "number='%d'", *dtoIn->number);
      }

      return createDtoResponse(Status::CODE_200, dtoOut);
  }
  
  ENDPOINT("POST", "/api/v1/keys/{master_SAE_ID}/dec_keys", getKeyWithID,
          PATH(String, master_SAE_ID),
          BODY_DTO(Object<kms::keySupply::dto::GetKeyWithIDDto>, /* unused */ getKeyDto))
  {
    auto dtoOut = kms::keySupply::dto::SendKeyDto::createShared();
    
    if (master_SAE_ID.get()) {
          OATPP_LOGD("Test", "master_SAE_ID='%s'", master_SAE_ID->c_str());
      } else {
          OATPP_LOGD("Test", "master_SAE_ID is null");
      }
    return createDtoResponse(Status::CODE_200, dtoOut);
  }

// private:
  std::shared_ptr<kms::keypool::manager::KeyPoolManager> m_keyPoolMgr;

};

#include OATPP_CODEGEN_END(ApiController) 

}}}

#endif /* kms_keySupply_KeySupplyController_hpp */