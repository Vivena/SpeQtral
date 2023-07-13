cmake_minimum_required(VERSION 3.1)

set(project_name keySupplyInterface) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/SwaggerComponent.hpp
        src/dto/keySupplyDTOs.hpp
        src/controller/KeySupplyInterfaceController.hpp
        src/Runner.cpp
        src/Runner.hpp
)

## include directories

target_include_directories(${project_name}-lib PUBLIC src)


## link libs

find_package(oatpp          1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC yaml-cpp
)

## define path to swagger-ui res folder

add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res")


## add executables

add_executable(${project_name}-exe src/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)

# add_executable(${project_name}-test
#         test/book-service/tests.cpp
# )

target_link_libraries(${project_name}-lib)

# target_link_libraries(${project_name}-test ${project_name}-lib)

# enable_testing()
# add_test(project-tests ${project_name}-test)