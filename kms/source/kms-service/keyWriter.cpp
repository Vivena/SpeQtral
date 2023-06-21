#include <string>
#include <sstream>
#include <jsoncpp/json/json.h>

using namespace Json;

/**
 * A utility class for serializing Key objects to strings.
 */
class KeyWriter {
public:
    /**
     * Returns the specified Key object as a string, formatted as follows:
     *
     * "<index>-<hexkey>-<blockid>"
     *
     * @param key The Key object to serialize to a string.
     * @return The specified Key object as a string.
     */
    static std::string toString(const Key& key) {
        std::ostringstream os;
        os << key.getIndex() << "-" << key.getHexKey() << "-" << key.getBlockID();
        return os.str();
    }

    /**
     * Returns the specified Key object as a JSON string, formatted as follows:
     *
     * {
     *   "index": <index>,
     *   "hex_key": "<hexkey>",
     *   "block_id": "<blockid>"
     * }
     *
     * @param key The Key object to serialize to a JSON string.
     * @return The specified Key object as a JSON string.
     */
    static std::string toJsonString(const Key& key) {
        Value root;
        root["index"] = key.getIndex();
        root["hex_key"] = key.getHexKey();
        root["block_id"] = key.getBlockID();

        FastWriter writer;
        return writer.write(root);
    }
};
