#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <filesystem>
#include <sstream>

#include "util/json.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

class Node {
public:
    Node(std::string id, std::string ipPort) : m_id(id), m_ipPort(ipPort) {}

    std::string getId() const { return m_id; }
    std::string getIpPort() const { return m_ipPort; }

private:
    std::string m_id;
    std::string m_ipPort;
};

class RouterConf {
public:
    explicit RouterConf(std::string filename) {
        loadFromFile(filename);
    }

    std::shared_ptr<Node> getNode(std::string id) const {
        auto it = m_nodes.find(id);
        if (it == m_nodes.end()) {
            throw std::invalid_argument("Node not found: " + id);
        }

        return it->second;
    }

    std::vector<std::shared_ptr<Node>> getAdjacentNodes(std::shared_ptr<Node> node) const {
        auto it = m_adjacentNodes.find(node);
        if (it != m_adjacentNodes.end()) {
            return it->second;
        } else {
            return {};
        }
    }

    bool isAdjacent(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) const {
        auto adjacentNodes = getAdjacentNodes(node1);
        return std::find(adjacentNodes.begin(), adjacentNodes.end(), node2) != adjacentNodes.end();
    }

private:
    void loadFromFile(const std::string& filename) {

        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Read the JSON data from the file
        nlohmann::json rootNode;
        file >> rootNode;

        auto nodes = rootNode["nodes"];
        for (auto& node : nodes) {
            std::string id = node["id"].get<std::string>();
            std::string ipPort = node["ip_port"].get<std::string>();

            m_nodes.emplace(id, std::make_shared<Node>(id, ipPort));
        }

        auto adjacentNodes = rootNode["adjacent"];
        for (auto& adjacentNode : adjacentNodes) {
            std::string nodeId = adjacentNode["node_id"].get<std::string>();

            std::vector<std::shared_ptr<Node>> adjacentNodes;
            auto adjacentIds = adjacentNode["adjacent_ids"];
            for (auto& adjacentId : adjacentIds) {
                adjacentNodes.push_back(m_nodes[adjacentId.get<std::string>()]);
            }

            m_adjacentNodes.emplace(m_nodes[nodeId], adjacentNodes);
        }
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Node>> m_nodes;
    std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> m_adjacentNodes;
};

