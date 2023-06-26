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
        : m_blockId(blockId)
        , m_blockSize(blockSize)
        , m_keys(keys)
    {
        m_index = std::make_shared<std::atomic<long>>(0);
    }

    explicit KeyPool(const KeyPool& other)
        : m_index(other.m_index)
        , m_blockId(other.m_blockId)
        , m_blockSize(other.m_blockSize)
        , m_keys(other.m_keys)
    {}

    /**
     * Returns the next unused key from the keyPool if possible.
     *
     * @return An object of type key.
     */
    Key getKey(){
        Key key;
        long index = m_index->fetch_add(1L, std::memory_order_acquire);
        if (m_keys.size() > 0)
            return Key(index, m_keys[index], m_blockId);
        
        key.setBlockID(m_blockId);
        return key;
    }

    /**
     * Returns the key at the index from the keyPool.
     *
     * @return An object of type key.
     */
    Key getKey(int index){
        Key key;
        if (index >= 0 && index < m_keys.size()){
            m_index->fetch_add(1L, std::memory_order_relaxed);
            return Key(index, m_keys.at(index), m_blockId);
        }

        key.setBlockID(m_blockId);
        m_index->fetch_add(1L, std::memory_order_relaxed);
        return key;
    }

    /**
     * Sets the ID of the block associated with this key to the specified value.
     * 
     * @param blockId The new ID of the block associated with this key.
     */
    void setBlockId(const std::string& blockId){
        m_blockId = blockId;
    }

    
    /**
     * Sets the size of the block associated with this key to the specified value.
     * 
     * @param blockSize The new size of the block associated with this key.
     */
    void setBlockSize(std::int64_t blockSize){
        m_blockSize = blockSize;
    }
    
    /**
     * Resets the counter used to track the number of times this key has been used.
     */
    void resetCounter(){
        m_index->exchange(0, std::memory_order_relaxed);
    }
    
    /**
     * Returns a reference to the vector of keys associated with this key pool.
     * 
     * @return A const reference to the vector of keys.
     */
    const std::vector<std::string>& getKeys() const{
        return m_keys;
    }
    
    /**
     * Returns the ID of the block associated with this key.
     * 
     * @return A const reference to the ID of the block.
     */
    const std::string& getBlockId() const{
        return m_blockId;
    }
    
    /**
     * Checks whether this key is valid - that is, whether it has not been used more
     * than its allowed number of times and whether it has at least one associated key.
     * 
     * @return True if the key is valid, false otherwise.
     */
    bool isValid() const{
        if (m_keys.empty()) {
            return false;
        }
        long cnt = m_index->load(std::memory_order_relaxed);
        return (cnt < m_blockSize);
    }
    
private:
    std::shared_ptr<std::atomic<long>> m_index;
    std::string m_blockId;
    std::int64_t m_blockSize;
    std::vector<std::string> m_keys;
};
