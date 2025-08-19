#pragma once
#include <vector>
#include <string>
#include "base/GameObject.h"

namespace ocf {

class AutoreleasePool {
public:
    AutoreleasePool();
    AutoreleasePool(std::string_view name);
    ~AutoreleasePool();


    void addObject(GameObject* pObject);

    void clear();

    bool isClearing() const { return m_isClearing; }

    bool contains(GameObject* pObject) const;

    void dump();

private:
    std::vector<GameObject*> m_managedObjects;
    std::string m_name;
    bool m_isClearing;
};

class PoolManager {
public:
    static PoolManager* getInstance();
    static void destroyInstance();

    AutoreleasePool* getCurrentPool() const;
 
    bool isObjectInPools(GameObject* pObject) const;

private:
    PoolManager();
    ~PoolManager();

    void push(AutoreleasePool* pool);
    void pop();

    static PoolManager* s_sheredPoolManager;

    std::vector<AutoreleasePool*> m_releasePoolStack;

    friend class AutoreleasePool;
};

} // namespace ocf
