cmake_minimum_required(VERSION 3.1)

set(project_name keyStore) # Rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

# Link libraries

find_package(oatpp 1.3.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_library(${project_name}-lib STATIC
    src/key/key.hpp
    src/key/keyWriter.hpp
    src/key/keyWriter.cpp
    src/keyStorage/keyPool.hpp
)

target_link_libraries(${project_name}-lib PUBLIC oatpp::oatpp yaml-cpp)

# Include directories

target_include_directories(${project_name}-lib 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)
