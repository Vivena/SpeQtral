#ifndef KEYWRITER_HPP
#define KEYWRITER_HPP

#include <string>
#include "key.hpp"

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
    static std::string toString(const Key& key);

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
    static std::string toJsonString(const Key& key);
};

#endif // KEYWRITER_HPP
