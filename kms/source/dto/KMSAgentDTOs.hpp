#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class SendStatusDto : public oatpp::DTO {
  
  DTO_INIT(SendStatusDto, DTO)

  
  DTO_FIELD(String, source_KME_ID);
  DTO_FIELD(String, target_KME_ID);
  DTO_FIELD(String, master_SAE_ID);
  DTO_FIELD(String, slave_SAE_ID);
  DTO_FIELD(Int32, key_size);
  DTO_FIELD(Int32, stored_key_coun);
  DTO_FIELD(Int32, max_key_count);
  DTO_FIELD(Int32, max_key_per_request);
  DTO_FIELD(Int32, max_key_size);
  DTO_FIELD(Int32, min_key_size);
  DTO_FIELD(Int32, max_SAE_ID_count );

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
  
};

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class GetKeyDto : public oatpp::DTO {
  
  DTO_INIT(GetKeyDto, DTO)
  
  DTO_FIELD_INFO(number) {
    info->required = false;
  }
  DTO_FIELD(Int32, number)= nullptr;

  DTO_FIELD_INFO(size) {
    info->required = false;
  }
  DTO_FIELD(Int32, size)= nullptr;

  DTO_FIELD_INFO(additional_slave_SAE_IDs) {
    info->required = false;
  }
  DTO_FIELD(Vector<String>, additional_slave_SAE_IDs)= nullptr;

  DTO_FIELD_INFO(extension_mandatory) {
    info->required = false;
  }
  DTO_FIELD(Vector<String>, extension_mandatory)= nullptr;

};

class Key_IDs : public oatpp::DTO {

  DTO_INIT(Key_IDs, DTO)

  DTO_FIELD(String, key_ID);
  DTO_FIELD(Any, key_ID_extension) = nullptr;

};


class GetKeyWithIDDto : public oatpp::DTO {
  DTO_INIT(GetKeyWithIDDto, DTO)

  DTO_FIELD(List<Object<Key_IDs>>, key_IDs);
  DTO_FIELD(Any, key_IDs_extension) = nullptr;

};


class Keys : public oatpp::DTO {

  DTO_INIT(Keys, DTO)

  DTO_FIELD(String, key_ID);
  DTO_FIELD(Any, key_ID_extension) = nullptr;
  DTO_FIELD(String, key);
  DTO_FIELD(Any, key_extension) = nullptr;

};

class SendKeyDto : public oatpp::DTO {
  DTO_INIT(SendKeyDto, DTO)
  
  DTO_FIELD(List<Object<Keys>>, keys);
  DTO_FIELD(Any, key_container_extension ) = nullptr;

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
};



#include OATPP_CODEGEN_END(DTO)

