#include "key.hpp"

long Key::getIndex() const { 
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_index; 
}

void Key::setIndex(int index) { 
    std::lock_guard<std::mutex> lock(m_mutex);
    m_index = index;
}

const std::string& Key::getHexKey() const { 
    return m_hexKey; 
}

void Key::setHexKey(const std::string& hexKey) { 
    m_hexKey = hexKey; 
}

const std::string& Key::getBlockID() const { 
    return m_blockID; 
}

void Key::setBlockID(const std::string& blockID) { 
    m_blockID = blockID; 
}
