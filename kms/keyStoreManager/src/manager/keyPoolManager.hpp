#pragma once

#include <unordered_map>
#include <mutex>
#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <condition_variable>

#include <yaml-cpp/yaml.h>

#include <oatpp/core/base/CommandLineArguments.hpp>


#include "../../../keyStore/src/keyStorage/keyPool.hpp"
#include "keyPoolManagerDefaultConfig.hpp"

namespace kms { namespace keypool { namespace manager { 

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

public:

    KeyPoolManager()
    :   m_byteSz{DEFAULT_BYTE_SZ}
    ,   m_blockSz{DEFAULT_BLOCK_SZ}
    ,   m_localSiteId{DEFAULT_LOCAL_SITE_ID}
    ,   m_poolsDir{DEFAULT_POOLS_DIR}
    ,   m_qnlIP{DEFAULT_QNL_IP}
    ,   m_qnlPort{DEFAULT_QNL_PORT}{}

    // KeyPoolManager(int byteSz, long blockSz, const std::string& localSiteId,
    // const std::string& poolsDir, const std::string& qnlIP, int qnlPort)
    // : m_byteSz{byteSz}, m_blockSz{blockSz}, m_localSiteId{localSiteId},
    //     m_poolsDir{poolsDir}, m_qnlIP{qnlIP}, m_qnlPort{qnlPort} {}

    // // TODO: allow for local config to be modified via args or config file
    // KeyPoolManager(oatpp::base::CommandLineArguments args, YAML::Node config)
    // :m_byteSz{DEFAULT_BYTE_SZ}
    // ,   m_blockSz{DEFAULT_BLOCK_SZ}
    // ,   m_localSiteId{DEFAULT_LOCAL_SITE_ID}
    // ,   m_poolsDir{DEFAULT_POOLS_DIR}
    // ,   m_qnlIP{DEFAULT_QNL_IP}
    // ,   m_qnlPort{DEFAULT_QNL_PORT}{}

    // return the size of a single key in bytes
    int getKeyByteSize() const{
        return m_byteSz;
    }

    // return the size of a key block in bytes
    long getKeyBlockSize() const{
        return m_blockSz;
    }

    // return the local site ID
    std::string getLocalSiteId() const{
        return m_localSiteId;
    }

    // create a new key and return it
    Key newKey(const std::string siteId){
        try {
            if (siteId.empty()) {
                throw std::invalid_argument("Site ID cannot be empty");
            }
            Key key = fetchKey(siteId, "", -1L);
            return key;
        }
        catch (const std::exception& e) {
            std::cerr << "An error occurred while creating a new Key: " << e.what() << std::endl;
            throw;
        }
    }

    // get a key at a specific index within a block for the given site
    Key getKey(const std::string& siteId, const std::string& block, long index){
        try {
            if (siteId.empty()) {
                throw std::invalid_argument("Site ID cannot be empty");
            }
            if (block.empty()) {
                throw std::invalid_argument("Block must not be empty");
            }
            if (index < 0) {
                throw std::out_of_range("Index must be non-negative");
            }
            Key key = fetchKey(siteId, block, index);
            return key;
        }
        catch (...) {
            std::cerr << "An error occurred while getting a Key" << std::endl;
            throw;
        }
    }


private:

    bool containsPoolLock(const std::string& poolName) const{
        return (m_poolLocks.find(poolName) != m_poolLocks.end());
    }

    std::shared_ptr<PoolLock> keyPoolLock(const std::string& poolName){
        auto it = m_poolLocks.find(poolName);
        if (it == m_poolLocks.end()) {
            // Create a new lock for the pool
            auto newLock = std::make_shared<PoolLock>();
            m_poolLocks.insert(make_pair(poolName, newLock));
            return newLock;
        }
        else {
            return it->second;
        }
    }

    Key fetchKey(const std::string& siteId, const std::string& inBlockId, long ind){
        std::string blockId = inBlockId.empty() ? "" : inBlockId;
        std::string srcSiteId = (ind < 0) ? m_localSiteId : siteId;
        std::string dstSiteId = (ind < 0) ? siteId : m_localSiteId;
        std::string poolName = srcSiteId + dstSiteId;

        bool isInProgress = false;

        // Check if a valid key pool exists for this site pair and it has keys available
        if (containsPool(poolName)) {
            return key(poolName, ind);
        }

        // Check if another thread is already fetching the key pool
        if (containsPoolLock(poolName)) {
            try {
                auto lock = keyPoolLock(poolName);
                std::unique_lock<std::mutex> lck(lock->lock);
                while (lock->inProgress) {
                    lock->signal.wait(lck);
                }
                lock->inProgress = true;
            }
            catch (const std::runtime_error& e) {
                std::cerr << "An error occurred while locking the KeyPoolManager: " << e.what() << std::endl;
                throw;
            }
        }
        
        else {
            // Create a new lock for the pool and mark it as in progress
            auto newPoolLock = std::make_shared<PoolLock>();
            newPoolLock->inProgress = true;
            m_poolLocks.insert(make_pair(poolName, newPoolLock));
        }

        // Acquire initialization lock and ensure that no other thread initialized the pool
        std::unique_lock<std::mutex> initPoolLck(m_initPoolMutex);
        if (containsPool(poolName)) {
            if (isInProgress) {
                keyPoolLock(poolName)->signal.notify_all();
            }
            return key(poolName, ind);
        }
        if (containsPoolLock(poolName) && keyPoolLock(poolName)->inProgress) {
            auto lock = keyPoolLock(poolName);
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
            // Do the actual fetching work
            std::vector<std::string> keyBlockDst;
            if (blockId.empty() && ind < 0) {
                // Block ID and index not specified - try to read from remote server
                // blockId = QNLKeyReader->read(m_localSiteId, siteId, keyBlockDst,
                //     m_poolsDir, m_poolsDir, m_poolsDir, static_cast<int>(m_blockSz), m_byteSz);
                if (blockId.empty()) {
                    throw std::runtime_error("Failed to read a valid key block from remote server.");
                }
            }
            else {
                // Block ID and/or index specified - read from local storage
                // QNLKeyReader->read(siteId, m_localSiteId, blockId, keyBlockDst, m_poolsDir, m_blockSz);
            }
            // Create a new key pool for this site pair and mark the lock as no longer in progress
            KeyPool kp(blockId, m_blockSz, keyBlockDst);
            m_keyPools.emplace(poolName, std::make_shared<KeyPool>(kp));
            if (isInProgress) {
                keyPoolLock(poolName)->signal.notify_all();
            } else {
                m_poolLocks[poolName]->inProgress = false;
            }
            return key(poolName, ind);
        }
        catch (...) {
            // Cleanup on failure and re-throw the exception
            if (isInProgress) {
                keyPoolLock(poolName)->signal.notify_all();
            }
            else {
                m_poolLocks.erase(poolName);
                m_keyPools.erase(poolName);
            }
            throw;
        }
    }

    bool containsPool(const std::string& poolName) const{
        auto it = m_keyPools.find(poolName);
        if (it == m_keyPools.end()) {
            return false;
        }
        return it->second->isValid();
    }

    Key key(const std::string& poolName, int index){
        auto it = m_keyPools.find(poolName);
        if (it == m_keyPools.end()) {
            throw std::runtime_error("Key not found");
        }
        if(index < 0)
            return it->second->getKey();

        return it->second->getKey(index);
    }

    int m_byteSz;
    long m_blockSz;
    std::string m_localSiteId;
    std::string m_poolsDir;
    std::string m_qnlIP;
    int m_qnlPort;

    std::unordered_map<std::string, std::shared_ptr<PoolLock>> m_poolLocks;
    std::unordered_map<std::string, std::shared_ptr<KeyPool>> m_keyPools;
    mutable std::mutex m_initPoolMutex;


};

}}}

