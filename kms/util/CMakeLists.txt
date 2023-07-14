cmake_minimum_required(VERSION 3.1)

set(project_name util) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

## link libs

find_package(oatpp 1.3.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_library(${project_name}-lib STATIC
        src/json.hpp
        src/Util.hpp
        src/Util.cpp
        src/ServerDefaultConfig.hpp
)

target_link_libraries(${project_name}-lib 
        PUBLIC oatpp::oatpp 
        PUBLIC yaml-cpp
)

## include directories

target_include_directories(${project_name}-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include directories

target_include_directories(${project_name}-lib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)


