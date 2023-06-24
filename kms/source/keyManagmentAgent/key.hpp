#ifndef KEY_HPP
#define KEY_HPP
#include <mutex>

#define EMPTY_KEY_INDEX -2

/**
 * Represents a cryptographic key that consists of an index, a hex key, and a block ID.
 */
class Key {
public:
    /**
     * Constructs a new default Key object.
     */
    Key()
        : m_index(EMPTY_KEY_INDEX)
        , m_hexKey("")
        , m_blockID("")
    {}

    /**
     * Constructs a new Key object with the given index, hex key, and block ID.
     *
     * @param index The index of the key.
     * @param hexKey The hexadecimal representation of the key.
     * @param blockID The ID of the block associated with the key.
     */
    Key(long index, const std::string& hexKey, const std::string& blockID)
        : m_index(index)
        , m_hexKey(hexKey)
        , m_blockID(blockID)
    {}

    /**
     * Copy constructor for Key class
     *
     * @param other The Key object to be copied.
     */
    Key(const Key& other)
        : m_index(other.m_index)
        , m_hexKey(other.m_hexKey)
        , m_blockID(other.m_blockID)
    {}


    /**
     * Returns the index of the key.
     *
     * @return The index of the key.
     */
    long getIndex() const;

    /**
     * Sets the index of the key to the specified value.
     *
     * @param index The new index of the key.
     */
    void setIndex(int index);

    /**
     * Returns the hexadecimal representation of the key.
     *
     * @return The hexadecimal representation of the key.
     */
    const std::string& getHexKey() const;

    /**
     * Sets the hexadecimal representation of the key to the specified value.
     *
     * @param hexKey The new hexadecimal representation of the key.
     */
    void setHexKey(const std::string& hexKey);

    /**
     * Returns the ID of the block associated with this key.
     *
     * @return The ID of the block associated with this key.
     */
    const std::string& getBlockID() const;

    /**
     * Sets the ID of the block associated with this key to the specified value.
     *
     * @param blockID The new ID of the block associated with this key.
     */
    void setBlockID(const std::string& blockID);

private:
    mutable std::mutex m_mutex;
    long m_index;
    std::string m_hexKey;
    std::string m_blockID;
};

#endif /*KEY_HPP*/