#include "keyPool.hpp"

Key KeyPool::getKey() {
    Key key;
    if (!m_keys.empty() && m_index < m_keys.size()) { 
        key.setHexKey(m_keys[m_index.fetch_add(1, std::memory_order_relaxed)]); 
        key.setIndex(m_index - 1);
    }
    key.setBlockID(m_blockId);
    return key;
}

Key KeyPool::getKey(const int index) {
    Key key;
    if (index >= 0 && index < m_keys.size()) {
        key.setIndex(index);
        key.setHexKey(m_keys.at(index));
    }
    key.setBlockID(m_blockId);
    m_index.fetch_add(1, std::memory_order_relaxed);
    return key;
}

void KeyPool::setBlockId(const std::string& block_id) {
    m_blockId = block_id;
}

void KeyPool::setBlockSize(const long block_size) {
    m_blockSize = block_size;
}

void KeyPool::resetCounter() {
    m_index.exchange(0, std::memory_order_relaxed);
}

const std::vector<std::string>& KeyPool::getKeys() const {
    return m_keys;
}

const std::string& KeyPool::getBlockId() const {
    return m_blockId;
}

bool KeyPool::isValid() const {
    if (m_keys.empty()) {
        return false;
    }
    long cnt = m_index.load(std::memory_order_relaxed);
    return (cnt < m_blockSize);
}

