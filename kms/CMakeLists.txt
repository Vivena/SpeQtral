cmake_minimum_required(VERSION 3.1)

set(project_name SpeQtral-QKD) ## rename your project here
set(CMAKE_VERBOSE_MAKEFILE ON)
project(${project_name})

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 11)

set(src_folder source)

## link libs

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_library(${project_name}-lib
        ${src_folder}/AppComponent.hpp
        ${src_folder}/keyManagmentAgent/keyStorage/keyPoolManager.hpp
        ${src_folder}/keySupplyAgent/keySupplyAgent.hpp
        ${src_folder}/dto/QNLAgentDTOs.hpp
        ${src_folder}/keyManagmentAgent/dto/KMSAgentDTOs.hpp
)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-swagger
        PUBLIC yaml-cpp
)

## define path to swagger-ui res folder

add_definitions( 
-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res"
)

target_include_directories(${project_name}-lib PUBLIC src)


target_include_directories(${project_name}-lib PUBLIC source)

## add executables

add_executable(${project_name}-exe
        ${src_folder}/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

# add_executable(${project_name}-test
#         test/tests.cpp
#         test/app/TestComponent.hpp
#         test/app/MyApiTestClient.hpp
#         test/MyControllerTest.cpp
#         test/MyControllerTest.hpp
# )

# target_link_libraries(${project_name}-test ${project_name}-lib)
target_link_libraries(${project_name}-lib)
# add_dependencies(${project_name}-test ${project_name}-lib)
# add_dependencies(${project_name}-lib)

# set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
set_target_properties(${project_name}-lib ${project_name}-exe  PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS ON
        CXX_STANDARD_REQUIRED ON
)

# enable_testing()
# add_test(project-tests ${project_name}-test)