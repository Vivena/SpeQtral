#pragma once

#include <thread>
#include <chrono>

#include "oatpp/core/Types.hpp"
#include "oatpp/network/Address.hpp"
#include "oatpp/network/tcp/Connection.hpp"


#include <iostream>
#include <string>
#include <vector>

#include "keyManagmentAgent/quantumNetworkLayer/clientInitializer.hpp"

/**
 * @brief A utility class that reads keys from the QNL.
 */
class QNLKeyReader {
public:
    /**
     * @brief Read a string representing keys from the QNL.
     *
     * @param src The source of the keys.
     * @param dest The destination of the keys.
     * @param keys A vector of integer keys to be read.
     * @param ip The IP address of the QNL server.
     * @param port The port number on which the QNL server is running.
     * @param poolBaseDir The name of the pool base directory.
     * @param blockSize The size of the blocks to be read.
     * @param byteSize The number of bytes to be read.
     *
     * @return A string representing the block ID.
     */
    static std::string readKeys(const std::string& src, const std::string& dest,
                                const std::vector<int>& keys, const std::string& ip,
                                int port, const std::string& poolBaseDir,
                                int blockSize, int byteSize);

      /**
     * @brief Connect to the QNL and read a string representing new keys from it.
     *
     * @param src The source of the keys.
     * @param dest The destination of the keys.
     * @param keys A vector of integer keys to be read.
     * @param ip The IP address of the QNL server.
     * @param port The port number on which the QNL server is running.
     * @param poolBaseDir The name of the pool base directory.
     * @param blockSize The size of the blocks to be read.
     * @param byteSize The number of bytes to be read.
     *
     * @return A string representing the block ID.
     */
    std::string connect(std::string src, std::string dest, std::vector<std::string> keys,
                    std::string ip, int port, std::string dir, long blockSz, int byteSz) throw(std::exception) {
        auto ci = make_shared<ClientInitializer>(src, dest, keys, (int)blockSz, byteSz);
        auto connector = oatpp::network::tcp::Connection::createShared();
        connector->setConnectionTimeout(std::chrono::milliseconds(20000));

        try {
            auto connection = connector->connect(ip.c_str(), port);
            auto& socket = connection->getSocket();

            auto config = oatpp::network::ClientConnectionProvider::Config::createShared(connection);
            auto provider = oatpp::network::ClientConnectionProvider::createShared(config);

            provider->getConnection(); // no need to close as per oatpp docs

        } catch(exception &e) {
            throw;
        }
        return ci->getBlockId(); 
    }

    /**
     * @brief Read a string representing an existing key from the QNL.
     *
     * @param src The source of the keys.
     * @param dest The destination of the keys.
     * @param keys A vector of integer keys to be read.
     * @param ip The IP address of the QNL server.
     * @param port The port number on which the QNL server is running.
     * @param poolBaseDir The name of the pool base directory.
     * @param blockSize The size of the blocks to be read.
     * @param byteSize The number of bytes to be read.
     *
     * @return A string representing the block ID.
     */
    static std::string readKeys(const std::string& src, const std::string& dest,
                                const std::vector<int>& keys, const std::string& ip,
                                int port, const std::string& poolBaseDir,
                                int blockSize);
};

std::string QNLKeyReader::readKeys(const std::string& src, const std::string& dest,
                                   const std::vector<int>& keys, const std::string& ip,
                                   int port, const std::string& poolBaseDir,
                                   int blockSize, int byteSize) {
    // Read keys from the QNL based on the provided parameters
    std::string keyStr = "";  // Example key string

    // Concatenate the list of keys into a single string
    for (auto key : keys) {
        keyStr += std::to_string(key) + ",";
    }
    // Remove the last comma from the string
    if (!keyStr.empty()) {
        keyStr.erase(keyStr.end() - 1);
    }

    // Return the key string
    return keyStr;
}


std::string QNLKeyReader::connect(std::string src, std::string dest, std::vector<std::string> keys,
                    std::string ip, int port, std::string dir, long blockSz, int byteSz) throw(std::exception) {
        auto ci = make_shared<ClientInitializer>(src, dest, keys, (int)blockSz, byteSz);
        auto connector = oatpp::network::tcp::Connector::createShared();
        connector->setConnectionTimeout(std::chrono::milliseconds(20000));

        try {
            auto connection = connector->connect(ip.c_str(), port);
            auto& socket = connection->getSocket();

            auto config = oatpp::network::ClientConnectionProvider::Config::createShared(connection);
            auto provider = oatpp::network::ClientConnectionProvider::createShared(config);

            provider->getConnection(); // no need to close as per oatpp docs

        } catch(exception &e) {
            throw;
        }
        return ci->getBlockId(); 
    }
