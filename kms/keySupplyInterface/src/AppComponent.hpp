#ifndef kms_keySupply_AppComponent_hpp
#define kms_keySupply_AppComponent_hpp

#include "Constants.hpp"

#include "SwaggerComponent.hpp"

#include "../../keyStoreManager/src/manager/keyPoolManager.hpp"

#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/component.hpp"

namespace kms { namespace keySupply {

struct HostPort {
  oatpp::String host;
  v_uint16 port;
};

class AppComponent {
private:
  HostPort m_hostPort;
  HostPort m_virtualHost;

public:

  AppComponent(const HostPort& hostPort, const HostPort& virtualHost)
    : m_hostPort(hostPort)
    , m_virtualHost(virtualHost)
  {}



  /**
   *  Swagger component
   */
  SwaggerComponent swaggerComponent;

  /**
   * Create virtualhost interface
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtualInterface)(Qualifiers::INTERFACE_KEYSUPPLY, [this] {
    return oatpp::network::virtual_::Interface::obtainShared(m_virtualHost.host);
  }());

  /**
   * Create "real-port" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)(Qualifiers::INTERFACE_KEYSUPPLY, [this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({m_hostPort.host, m_hostPort.port});
  }());

  /**
   * Create "virtualhost" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, virtualConnectionProvider)(Qualifiers::INTERFACE_KEYSUPPLY_VH, [] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, interface, Qualifiers::INTERFACE_KEYSUPPLY);
    return oatpp::network::virtual_::server::ConnectionProvider::createShared(interface);
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(Qualifiers::INTERFACE_KEYSUPPLY, [] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)(Qualifiers::INTERFACE_KEYSUPPLY, [] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

  // /**
  //  *  Create Demo-Database component which stores information about books
  //  */
  // OATPP_CREATE_COMPONENT(std::shared_ptr<db::Database>, database)(Qualifiers::INTERFACE_KEYSUPPLY, [] {
  //   return std::make_shared<db::Database>();
  // }());

};

}}

#endif /* example_book_AppComponent_hpp */