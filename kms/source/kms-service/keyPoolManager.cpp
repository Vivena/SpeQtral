#include <vector>
#include <unordered_map>
#include <mutex>
#include <condition_variable>

class Key {
    // implementation details not shown
};

class KeyPool {
public:
    KeyPool(std::string blockId, long blockSz, std::vector<std::string> keyBlockDst) :
        m_blockId(blockId), m_blockSz(blockSz), m_keyBlockDst(keyBlockDst) {}

    bool isValid() const { return !m_keyBlockDst.empty(); }

    Key getKey() const { return getKey(0); }

    Key getKey(int index) const {
        if (index < 0 || index >= static_cast<int>(m_keyBlockDst.size()))
            return Key();
        else
            return Key(m_keyBlockDst[index]);
    }

private:
    std::string m_blockId;
    long m_blockSz;
    std::vector<std::string> m_keyBlockDst;
};

class KeyPoolManager {
public:
    KeyPoolManager() = default;

    int getKeyByteSize() const { return m_byteSz; }
    long getKeyBlockSize() const { return m_blockSz; }
    std::string getLocalSiteId() const { return m_localSiteId; }

    Key newKey(const std::string& siteId) {
        try {
            return fetchKey(siteId, "", -1);
        } catch (const std::exception&) {
            return Key();
        }
    }

    Key getKey(const std::string& siteId, const std::string& block, long index) {
        try {
            return fetchKey(siteId, block, index);
        } catch (const std::exception&) {
            return Key();
        }
    }

private:
    class PoolLock {
    public:
        bool inProgress{false};
        std::mutex mutex;
        std::condition_variable cond;
    };

    std::unordered_map<std::string, KeyPool> m_keyPools;
    std::unordered_map<std::string, PoolLock> m_poolLocks;
    mutable std::mutex m_initPoolMutex;
    int m_byteSz{0};
    long m_blockSz{0};
    std::string m_localSiteId;
    std::string m_poolsDir;
    std::string m_qnlIP;
    int m_qnlPort{0};

    Key fetchKey(const std::string& siteId, const std::string& inBlockId, long ind) {
        std::string blockId = inBlockId;
        Key cipherKey;
        std::string srcSiteId;
        std::string dstSiteId;
        std::string poolName;
        int index = static_cast<int>(ind);

        if (index < 0) {
            srcSiteId = m_localSiteId;
            dstSiteId = siteId;
        } else {
            srcSiteId = siteId;
            dstSiteId = m_localSiteId;
        }

        poolName = srcSiteId + dstSiteId;

        {
            std::unique_lock<std::mutex> lock(m_initPoolMutex);
            while (!containsPool(poolName) && containsPoolLock(poolName) &&
                   keyPoolLock(poolName).inProgress) {
                // Wait for another thread to finish initializing the key pool
                keyPoolLock(poolName).cond.wait(lock);
            }
            if (containsPool(poolName)) {
                cipherKey = key(poolName, index);
                return cipherKey;
            } else if (containsPoolLock(poolName) &&
                       keyPoolLock(poolName).inProgress) {
                // Wait for another thread to finish initializing the key pool
                keyPoolLock(poolName).cond.wait(lock, [&]() {
                    return !keyPoolLock(poolName).inProgress;
                });
                cipherKey = key(poolName, index);
                return cipherKey;
            } else {
                // Create a new pool lock
                PoolLock& poolLock = m_poolLocks[poolName];
                poolLock.inProgress = true;

                lock.unlock();

                // Fetch the key block from the remote site
                std::vector<std::string> keyBlockDst(m_blockSz);
                try {
                    if (blockId.empty() && index == -1) {
                        blockId = keyReader.read(m_localSiteId, siteId, keyBlockDst,
                                                 m_qnlIP, m_qnlPort, m_poolsDir, m_blockSz, m_byteSz);
                    } else {
                        keyReader.read(siteId, m_localSiteId, blockId, keyBlockDst, m_poolsDir, m_blockSz);
                    }
                } catch (const std::exception& e) {
                    throw e;
                }

                // Create a new key pool and add it to the map
                KeyPool kp(blockId, m_blockSz, keyBlockDst);
                m_keyPools.emplace(poolName, std::move(kp));

                {
                    std::lock_guard<std::mutex> lock(poolLock.mutex);
                    poolLock.inProgress = false;
                    poolLock.cond.notify_all();
                }

                cipherKey = key(poolName, index);
                return cipherKey;
            }
        }
    }

    bool containsPool(const std::string& poolName) const {
        auto it = m_keyPools.find(poolName);
        if (it != m_keyPools.end()) {
            const KeyPool& kp = it->second;
            return kp.isValid();
        }
        return false;
    }

    Key key(const std::string& poolName, int index) const {
        auto it = m_keyPools.find(poolName);
        if (it != m_keyPools.end()) {
            const KeyPool& kp = it->second;
            return kp.getKey(index);
        } else {
            return Key();
        }
    }

    bool containsPoolLock(const std::string& poolName) const {
        return m_poolLocks.count(poolName) > 0;
    }

    PoolLock& keyPoolLock(const std::string& poolName) {
        return m_poolLocks[poolName];
    }

    // other member variables and functions not shown
};

int main() {
    KeyPoolManager kpm;
    // configure the KeyPoolManager instance
    // ...
    Key k = kpm.newKey("someSiteId");
    // use the Key object
    // ...
    return 0;
}
