#pragma once

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"


#include "dto/DTOs.hpp"
#include "keyManagmentAgent/keyStorage/keyPoolManager.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class KeySupplyAgent : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  KeySupplyAgent(OATPP_COMPONENT(std::shared_ptr<ObjectMapper> objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:

  ENDPOINT("GET","/api/v1/keys/{slave_SAE_ID}/status", getStatus, PATH(String, slave_SAE_ID)){
    auto dto = SendStatusDto::createShared();
    OATPP_LOGD("Test", "slave_SAE_ID='%s'", slave_SAE_ID->c_str());
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("POST", "/api/v1/keys/{slave_SAE_ID}/enc_keys", getKey,\
          PATH(String, slave_SAE_ID),\
          BODY_DTO(Object<GetKeyDto>, getKeyDto))
  {
      auto dtoIn = getKeyDto;
      auto dtoOut = SendKeyDto::createShared();

      if (slave_SAE_ID) {
          OATPP_LOGD("Test", "slave_SAE_ID='%s'", slave_SAE_ID->c_str());
          auto k = m_keyPoolManager.newKey(*slave_SAE_ID);
          // TODO: add a check on the slave_SAE_ID
      } else {
          return createDtoResponse(Status::CODE_200, dtoOut);
      }


      if (dtoIn->number) {
        OATPP_LOGD("Test", "number='%d'", *dtoIn->number);
      } else{
        dtoIn->number = 1;
        OATPP_LOGD("Test", "number='%d'", *dtoIn->number);
      }

      for (size_t i = 0; i < dtoIn->number; i++)
      {
        auto keyDto = SendKeyDto::createShared();

      }
      

      return createDtoResponse(Status::CODE_200, dtoOut);
  }
  
  ENDPOINT("POST", "/api/v1/keys/{master_SAE_ID}/dec_keys", getKeyWithID,\
          PATH(String, master_SAE_ID),\
          BODY_DTO(Object<GetKeyWithIDDto>, getKeyDto))
  {
    auto dtoOut = SendKeyDto::createShared();
    
    if (master_SAE_ID) {
          OATPP_LOGD("Test", "master_SAE_ID='%s'", master_SAE_ID->c_str());
      } else {
          OATPP_LOGD("Test", "master_SAE_ID is null");
      }
    return createDtoResponse(Status::CODE_200, dtoOut);
  }

private:
  KeyPoolManager m_keyPoolManager;

};

#include OATPP_CODEGEN_END(ApiController) 
