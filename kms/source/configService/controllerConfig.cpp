#include <memory>
#include <functional>
#include "controllerConfig.hpp"
#include "controller1.hpp" // Include statement for Controller1 class
#include "controller2.hpp" // Include statement for Controller2 class

std::shared_ptr<oatpp::web::app::Controller> ControllerConfig::getController() const { 
    return m_controller; 
}

const oatpp::String& ControllerConfig::getPort() const { 
    return m_port; 
}

const oatpp::String& ControllerConfig::getCertFilePath() const { 
    return m_certFile; 
}

const oatpp::String& ControllerConfig::getKeyFilePath() const { 
    return m_keyFile; 
}

oatpp::network::ServerConnectionProvider::Config ControllerConfig::createConnectionProviderConfig() const {
    return {
        m_port,
        {
            {"tls_certificate_file", m_certFile},
            {"tls_private_key_file", m_keyFile}
        }
    };
}

std::vector<std::shared_ptr<ControllerConfig>> ControllerConfig::parseControllerConfigsFromYamlFile(const YAML::Node& yaml) {
    // Validate YAML node
    if (!yaml.IsSequence()) {
        throw std::invalid_argument("Invalid YAML node. Expected a sequence.");
    }

    std::vector<std::shared_ptr<ControllerConfig>> configs;

    // Parse configuration values for all controllers from YAML nodes
    for (const auto& configYaml : yaml) {
        std::string controllerName;
        int port = -1;
        std::string certFile;
        std::string keyFile;

        // Validate YAML node for each controller
        if (!configYaml.IsMap()) {
            throw std::invalid_argument("Invalid YAML node. Expected a map.");
        }

        // Parse configuration values from YAML node
        if (auto it = configYaml["controller"]; it != configYaml.end() && it->IsScalar()) {
            controllerName = it->as<std::string>();
        } else {
            throw std::runtime_error("Controller name missing in config");
        }

        if (auto it = configYaml["port"]; it != configYaml.end() && it->IsScalar()) {
            port = it->as<int>();
        }

        if (auto it = configYaml["tls_certificate_file"]; it != configYaml.end() && it->IsScalar()) {
            certFile = it->as<std::string>();
        }

        if (auto it = configYaml["tls_private_key_file"]; it != configYaml.end() && it->IsScalar()) {
            keyFile = it->as<std::string>();
        }

        std::shared_ptr<oatpp::web::app::Controller> controller;

        // Create controller based on provided name
        if (controllerName == "Controller1") {
            controller = std::make_shared<Controller1>();
        } else if (controllerName == "Controller2") {
            controller = std::make_shared<Controller2>();
        } else {
            throw std::runtime_error("Unknown controller type: " + controllerName);
        }

        // Check if make_shared was successful
        std::shared_ptr<ControllerConfig> config;
        try {
            config = std::make_shared<ControllerConfig>(controller, port == -1 ? "" : std::to_string(port), certFile, keyFile);
        } catch (const std::bad_alloc& e) {
            throw std::runtime_error("Failed to allocate memory: " + std::string(e.what()));
        }

        configs.push_back(config);
    }
    
    return configs;
}



