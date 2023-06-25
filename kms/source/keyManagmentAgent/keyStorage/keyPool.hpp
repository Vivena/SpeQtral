#pragma once

#include <vector>
#include <atomic>
#include <string>
#include <cstdint>

#include "keyManagmentAgent/key.hpp"

/**
 * Represents a cryptographic keyPool that consists of an index, a block ID, and a block size and multiple keys.
 */
class KeyPool {
public:
    /**
     * Constructs a new KeyPool object with an index set to 0, and the given block ID, and block size, and key vector.
     *
     * @param blockId The block ID.
     * @param blockSize The siwe in bits of a block.
     * @param keys The vector of keys associated.
     */
    KeyPool(const std::string& blockId, std::int64_t blockSize, const std::vector<std::string>& keys)
        : m_index(0)
        , m_blockId(blockId)
        , m_blockSize(blockSize)
        , m_keys(keys)
    {}

    /**
     * Returns the next unused key from the keyPool if possible.
     *
     * @return An object of type key.
     */
    Key getKey();

    /**
     * Returns the key at the index from the keyPool.
     *
     * @return An object of type key.
     */
    Key getKey(int index);

    /**
     * Sets the ID of the block associated with this key to the specified value.
     * 
     * @param blockId The new ID of the block associated with this key.
     */
    void setBlockId(const std::string& blockId);
    
    /**
     * Sets the size of the block associated with this key to the specified value.
     * 
     * @param blockSize The new size of the block associated with this key.
     */
    void setBlockSize(std::int64_t blockSize);
    
    /**
     * Resets the counter used to track the number of times this key has been used.
     */
    void resetCounter();
    
    /**
     * Returns a reference to the vector of keys associated with this key pool.
     * 
     * @return A const reference to the vector of keys.
     */
    const std::vector<std::string>& getKeys() const;
    
    /**
     * Returns the ID of the block associated with this key.
     * 
     * @return A const reference to the ID of the block.
     */
    const std::string& getBlockId() const;
    
    /**
     * Checks whether this key is valid - that is, whether it has not been used more
     * than its allowed number of times and whether it has at least one associated key.
     * 
     * @return True if the key is valid, false otherwise.
     */
    bool isValid() const;
    
private:
    std::atomic<long> m_index;
    std::string m_blockId;
    std::int64_t m_blockSize;
    std::vector<std::string> m_keys;
};
