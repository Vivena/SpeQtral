#pragma once

#include "keyPool.hpp"

#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <unordered_map>

#include "util/util.hpp"

class KeyPoolManager {

public:

    class PoolLock {
    public:
        bool inProgress;
        std::mutex lock;
        std::condition_variable signal;

        PoolLock() 
            : inProgress(false)
            ,  signal()
        {}

        // destructor
        ~PoolLock() {
            if (inProgress) {
                inProgress = false;
                signal.notify_all();
            }
            lock.unlock();
        }
    };


    KeyPoolManager(int byteSz, long blockSz, const std::string& localSiteId,
                   const std::string& poolsDir, const std::string& qnlIP, int qnlPort) :
        byteSz(byteSz), blockSz(blockSz), localSiteId(localSiteId), poolsDir(poolsDir),
        qnlIP(qnlIP), qnlPort(qnlPort) {}

    // return the size of a single key in bytes
    int getKeyByteSize(){
        return byteSz;
    }

    // return the size of a key block in bytes
    long getKeyBlockSize(){
        return blockSz;
    }

    // return the local site ID
    std::string getLocalSiteId(){
        return localSiteId;
    }

    // create a new key and return it
    Key newKey(const std::string siteId){

        if (siteId.empty()) {
           throw std::invalid_argument("Site ID cannot be empty");
        }
        try {
            Key key = fetchKey(siteId, "", -1L);
            return key;
        } catch (const std::exception& e) {
            std::throw_with_nested(std::runtime_error("An error occurred while creating a new Key"));
        }
    }
    

    // get a key at a specific index within a block for the given site
    Key getKey(const std::string& siteId, const std::string& block, long index){
        if (siteId.empty()) {
            throw std::invalid_argument("Site ID cannot be empty");
        }

        if (block.empty()) {
            throw std::invalid_argument("Block must not be empty");
        }

        if (index < 0) {
            throw std::out_of_range("Index must be non-negative");
        }

        try {
            Key key = fetchKey(siteId, block, index);
            return key;
        } catch (...) {
            std::throw_with_nested(std::runtime_error("An error occurred"));
        }
    }
    
private:

    bool containsPoolLock(const std::string& poolName){
        return (m_poolLocks.find(poolName) != m_poolLocks.end());
    }

    std::shared_ptr<PoolLock> keyPoolLock(const std::string& poolName){
        auto it = m_poolLocks.find(poolName); // find the iterator for the given poolName
        if (it == m_poolLocks.end()) { // check if poolName not found in m_poolLocks
            // handle error or throw exception
            throw std::runtime_error("Pool name not found");
        }
        return it->second;
    }

    // fetch a key based on its index or the ID of the block it belongs to
    Key fetchKey(const std::string& siteId, const std::string& inBlockId, long ind){
        std::string blockId = inBlockId.empty() ? "" : inBlockId;
        std::string srcSiteId = (ind < 0) ? localSiteId : siteId;
        std::string dstSiteId = (ind < 0) ? siteId : localSiteId;
        std::string poolName = srcSiteId + dstSiteId;
        bool isInProgress = false;
    
        // check if a valid key pool exists for this site pair
        if (containsPool(poolName)) {
            return key(poolName, ind);
        }
    
        // check if another thread is already fetching the key pool
        if (containsPoolLock(poolName)) {
            try{
                std::shared_ptr<PoolLock> lock = keyPoolLock(poolName);
                
                std::unique_lock<std::mutex> lck(lock->lock);
                while (lock->inProgress) {
                    lock->signal.wait(lck);
                }
                isInProgress = true;
            } catch (const std::runtime_error& e) {
                throw e;
            }
        } else {
            std::shared_ptr<PoolLock> newPoolLock;
            m_poolLocks.insert(std::make_pair(poolName, newPoolLock));
        }
    
        // acquire initialization lock and ensure that no other thread initialized the pool
        std::unique_lock<std::mutex> initPoolLck(this->m_initPoolMutex);
        if (containsPool(poolName)) {
            if (isInProgress) {
                keyPoolLock(poolName)->signal.notify_all();
            }
            return key(poolName, ind);
        }
        if (containsPoolLock(poolName) && keyPoolLock(poolName)->inProgress) {
            std::shared_ptr<PoolLock> lock = keyPoolLock(poolName);
            std::unique_lock<std::mutex> lck(lock->lock);
            while (lock->inProgress) {
                lock->signal.wait(lck);
            }
            isInProgress = true;
        } else {
            m_poolLocks[poolName]->inProgress = true;
            isInProgress = false;
        }
    
        try {
            // do the actual fetching work
            std::vector<std::string> keyBlockDst;
            if (blockId.empty() && ind < 0) {
                blockId = keyReader.read(localSiteId, siteId, keyBlockDst,
                                         qnlIP, qnlPort, poolsDir, static_cast<int>(blockSz), byteSz);
                if (blockId.empty()) {
                    throw std::runtime_error("Failed to read a valid key block from key reader.");
                }
            } else {
                keyReader.read(siteId, localSiteId, blockId, keyBlockDst, poolsDir, blockSz);
            }
            KeyPool kp(blockId, blockSz, keyBlockDst);
            m_keyPools.emplace(poolName, kp);
            if (isInProgress) {
                keyPoolLock(poolName)->signal.notify_all();
            } else {
                m_poolLocks[poolName]->inProgress = false;
            }
            return key(poolName, ind);
        } catch (const std::exception&) {
            if (isInProgress) {
                keyPoolLock(poolName)->signal.notify_all();
            } else {
                m_poolLocks.erase(poolName);
                m_keyPools.erase(poolName);
            }
            return Key();
        }
    }
    

    // check if a valid key pool exists for the given site pair
    bool containsPool(const std::string& poolName){
        auto it = m_keyPools.find(poolName);
        if (it == m_keyPools.end()) {
            return false;
        }
        return it->second->isValid();
    }

    // get a key from the specified pool and index
    Key key(const std::string& poolName, int index){
        auto it = m_keyPools.find(poolName);
        if (it == m_keyPools.end()) {
            return Key();
        }
        return it->second->getKey(index);
    }

    
public: 
    std::unordered_map<std::string, std::shared_ptr<PoolLock>> m_poolLocks;

private:

    int byteSz;
    long blockSz;
    std::string localSiteId;
    std::string poolsDir;
    std::string qnlIP;
    int qnlPort;
    // QNLKeyReader keyReader;
    
    std::unordered_map<std::string, std::shared_ptr<KeyPool>> m_keyPools;
    std::mutex m_initPoolMutex;
};
