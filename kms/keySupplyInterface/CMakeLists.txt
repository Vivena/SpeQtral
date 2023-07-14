cmake_minimum_required(VERSION 3.1)

set(project_name keySupplyInterface) ## Rename your project here

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
# set(UTIL_DIR ${CMAKE_CURRENT_LIST_DIR}/../../util/build)

# FIND_LIBRARY(UTIL_LIBRARY
#         NAMES util-lib
#         HINTS ${UTIL_DIR}
# )

target_include_directories(${project_name}-lib
        PUBLIC src
        PUBLIC ../keyStore/src/keyStorage # Add this line to include keyStorage directory
        PUBLIC ../keyStore/src/key
)

# Link libraries
find_package(oatpp          1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

# Define the UTIL_LIBRARY and KEYSTORE_LIBRARY variables
# set(UTIL_LIBRARY ../util-lib)
# set(KEYSTORE_LIBRARY ../keyStore/keyStore-lib)
# set(KEYSTORE_LIBRARY ${KEYSTORE_DIR}/keyStore-lib)

target_link_libraries(${project_name}-lib
        PUBLIC ${UTIL_LIBRARY}
        PUBLIC ${KEYSTORE_LIBRARY}
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC yaml-cpp
)

# Define path to swagger-ui res folder
add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res")

# Add executables
add_executable(${project_name}-exe src/App.cpp)

# Link the project-specific library and keyStore-lib to the executable
target_link_libraries(${project_name}-exe PUBLIC ${project_name}-lib ${UTIL_LIBRARY} ${KEYSTORE_LIBRARY})

# Add dependency to keyStore-lib target
add_dependencies(${project_name}-exe ${project_name}-lib)



