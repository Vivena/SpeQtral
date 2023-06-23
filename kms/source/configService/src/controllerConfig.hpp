#include <yaml-cpp/yaml.h>
#include "oatpp/web/server/api/ApiController.hpp"

/**
 * Represents an Oat++ controller's config that consists of a shared pointer to an Oat++ 
 * web app controller, a port, a cert file, and a key file.
 */
class ControllerConfig {

public:

  /**
   * Constructor for the ControllerConfig class
   * @param controller A shared pointer to an Oat++ web app controller object
   * @param port A string representing the port number
   * @param certFile A string representing the path to the certificate file
   * @param keyFile A string representing the path to the key file
   */
  ControllerConfig(const std::shared_ptr<oatpp::web::server::api::ApiController>& controller,
                   const oatpp::String& port,
                   const oatpp::String& certFile,
                   const oatpp::String& keyFile)
    : m_controller(controller), m_port(port), m_certFile(certFile), m_keyFile(keyFile) {}

  /**
   * Getter method to get the value of the controller object
   * @return A shared pointer to an Oat++ web app controller object
   */
  std::shared_ptr<oatpp::web::server::api::ApiController> getController() const;
  
  /**
   * Getter method to get the value of the port number
   * @return A string representing the port number
   */
  const oatpp::String& getPort() const;
  
  /**
   * Getter method to get the value of the certificate file path
   * @return A string representing the path to the certificate file
   */
  const oatpp::String& getCertFilePath() const;
  
  /**
   * Getter method to get the value of the key file path
   * @return A string representing the path to the key file
   */
  const oatpp::String& getKeyFilePath() const;

  /**
   * Method to create and return ServerConnectionProvider Config object
   * @return A ServerConnectionProvider Config object based on the member variables
   */
  oatpp::network::ServerConnectionProvider::Config createConnectionProviderConfig() const;

  /**
   * Static method to parse a YAML node and create ControllerConfig object
   * @param yaml A reference to a YAML parsed node
   * @return A shared pointer to a ControllerConfig object
   */
  static std::shared_ptr<ControllerConfig> parseControllerConfigsFromYamlFile(const YAML::Node& yaml);

private:
  // Member variables of the class
  std::shared_ptr<oatpp::web::server::api::ApiController> m_controller; // shared pointer to web app controller
  const oatpp::String& m_port; // string to represent port
  const oatpp::String& m_certFile; // string to represent certificate file
  const oatpp::String& m_keyFile; // string to represent key file
};
