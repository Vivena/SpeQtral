#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class SendVersion : public oatpp::DTO {
  
  DTO_INIT(SendVersion, DTO)

  DTO_FIELD(List<String>, versions);
 
  
};

class Keys : public oatpp::DTO {

  DTO_INIT(Keys, DTO)

  DTO_FIELD(String, key_ID);
  DTO_FIELD(Any, key_ID_extension) = nullptr;
  DTO_FIELD(String, key);
  DTO_FIELD(Any, key_extension) = nullptr;

};

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class TransferKeyDto : public oatpp::DTO {
  
  DTO_INIT(TransferKeyDto, DTO)
  
  DTO_FIELD(List<Object<Keys>>, keys);
  DTO_FIELD(List<String>, additional_target_sae_ids);
//   DTO_FIELD(String, ack_callback_url);

};

class Keys_ID : public oatpp::DTO {

  DTO_INIT(Keys_ID, DTO)

  DTO_FIELD(String, key_ID);
  DTO_FIELD(Any, extension) = nullptr;

};

class ACK_containers : public oatpp::DTO {

  DTO_INIT(ACK_containers, DTO)

  DTO_FIELD(List<Keys_ID>, key_ids);
  DTO_FIELD(String, ack_status);
  DTO_FIELD(String, initiator_sae_id);
  DTO_FIELD(String, target_sae_id);
  DTO_FIELD(Any, extension) = nullptr;

};



#include OATPP_CODEGEN_END(DTO)

