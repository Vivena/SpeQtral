#ifndef example_book_AppComponent_hpp
#define example_book_AppComponent_hpp

#include "manager/keyPoolManager.hpp"

#include "Constants.hpp"

#include <yaml-cpp/yaml.h>

#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>

namespace kms { namespace keypool {

class AppComponent {

private:
    oatpp::base::CommandLineArguments m_args;
    YAML::Node m_config;

public:
    AppComponent(oatpp::base::CommandLineArguments args, YAML::Node config)
    : m_args(args)
    , m_config(config)
  {}

  /**
   *  Create Demo-Database component which stores information about books
   */
    // OATPP_CREATE_COMPONENT(std::shared_ptr<kms::keypool::manager::KeyPoolManager>, keyPoolManager)(Qualifiers::KEYPOOL_MANAGER, []{
    //     // Assurez-vous que les types et les valeurs des arguments sont corrects
    //     return std::make_shared<manager::KeyPoolManager>();
    // });
    
  

};

}}

#endif /* example_book_AppComponent_hpp */