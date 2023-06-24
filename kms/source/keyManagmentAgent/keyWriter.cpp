#include <sstream>
#include <jsoncpp/json/json.h>
#include "keyWriter.hpp"

using namespace Json;

std::string KeyWriter::toString(const Key& key) {
    std::ostringstream os;
    os << key.getIndex() << "-" << key.getHexKey() << "-" << key.getBlockID();
    return os.str();
}

std::string KeyWriter::toJsonString(const Key& key) {
    Value root;
    root["index"] = Json::Int64(key.getIndex());
    // root["index"] = key.getIndex();
    root["hex_key"] = key.getHexKey();
    root["block_id"] = key.getBlockID();

    FastWriter writer;
    return writer.write(root);
}