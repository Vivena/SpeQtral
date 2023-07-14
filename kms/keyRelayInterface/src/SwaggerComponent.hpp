
#ifndef kms_keyRelay_SwaggerComponent_hpp
#define kms_keyRelay_SwaggerComponent_hpp

#include "Constants.hpp"

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

#include "../../util/src/ServerDefaultConfig.hpp"
#include "../../util/src/Util.hpp"

namespace kms { namespace keyRelay {

    class SwaggerComponent {
    public:

    /**
   *  General API docs info
   */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)(Qualifiers::INTERFACE_KEYRELAY, [] {

            oatpp::swagger::DocumentInfo::Builder builder;

            builder
            .setTitle("My Demo Service with Swagger-UI")
            .setDescription("C++/oat++ Web Service with Swagger-UI")
            .setVersion("1.0")
            .setContactName("Edward Guyot")
            .setContactUrl("https://oatpp.io/")
            .setLicenseName("Apache License, Version 2.0")
            .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")

            .addServer("http://localhost:" + std::to_string(KEYRELAYAGENT_DEFAULT_PORT), "server on localhost");

            return builder.build();

        }());

        /**
         *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)(Qualifiers::INTERFACE_KEYRELAY, [] {
            // Make sure to specify correct full path to oatpp-swagger/res folder !!!
            return oatpp::swagger::Resources::streamResources(OATPP_SWAGGER_RES_PATH);
        }());

    };

}}

#endif // kms_keyRelay_Runner_hpp