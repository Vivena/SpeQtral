
#ifndef kms_keySupply_SwaggerComponent_hpp
#define kms_keySupply_SwaggerComponent_hpp

#include "Constants.hpp"

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

#include "../../util/src/ServerDefaultConfig.hpp"

namespace kms { namespace keySupply {

    class SwaggerComponent {
    public:

    /**
   *  General API docs info
   */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)(Qualifiers::INTERFACE_KEYSUPPLY, [] {

            oatpp::swagger::DocumentInfo::Builder builder;

            builder
            .setTitle(TITLE)
            .setDescription(DESCRIPTION)
            .setVersion(VERSION)
            .setContactName(CONTACT_NAME)
            .setContactUrl(CONTACT_URL)

            .setLicenseName("The Unlicense")

            .addServer("http://localhost:" + std::to_string(KEYSUPPLYAGENT_DEFAULT_PORT), "server on localhost");;

            return builder.build();

        }());

        /**
         *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)(Qualifiers::INTERFACE_KEYSUPPLY, [] {
            // Make sure to specify correct full path to oatpp-swagger/res folder !!!
            return oatpp::swagger::Resources::streamResources(OATPP_SWAGGER_RES_PATH);
        }());

    };

}}

#endif // kms_keySupply_Runner_hpp