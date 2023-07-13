cmake_minimum_required(VERSION 3.1)

set(project_name all-services) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_executable(${project_name}-exe
        App.cpp
)

find_package(oatpp          1.3.0 REQUIRED)
find_package(oatpp-swagger  1.3.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

target_link_libraries(${project_name}-exe
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC yaml-cpp
)

add_library(${project_name}-lib
        util/Util.hpp
        util/Util.cpp
)

## include directories

target_include_directories(${project_name}-lib PUBLIC util)
set(KEYSUPPLY_INTERFACE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../keySupplyInterface/build)
# set(BOOK_SERVICE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../book-service/build)
# set(FACADE_SERVICE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../facade/build)

FIND_LIBRARY(KEYSUPPLY_LIBRARY
        NAMES keySupplyInterface-lib
        HINTS ${KEYSUPPLY_INTERFACE_DIR}
)

# FIND_LIBRARY(BOOK_LIBRARY
#         NAMES book-service-lib
#         HINTS ${BOOK_SERVICE_DIR}
# )

# FIND_LIBRARY(FACADE_LIBRARY
#         NAMES facade-lib
#         HINTS ${FACADE_SERVICE_DIR}
# )

target_include_directories(${project_name}-exe
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../keySupplyInterface/src
        # PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../book-service/src
        # PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../facade/src
)

target_link_libraries(${project_name}-exe
        PUBLIC ${USER_LIBRARY}
        # PUBLIC ${BOOK_LIBRARY}
        # PUBLIC ${FACADE_LIBRARY}
)


## define path to swagger-ui res folder
add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res")
