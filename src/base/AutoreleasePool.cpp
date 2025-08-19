#include "AutoreleasePool.h"

namespace ocf {

AutoreleasePool::AutoreleasePool()
    : m_name("")
    , m_isClearing(false)
{
    m_managedObjects.reserve(128);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::AutoreleasePool(std::string_view name)
    : m_name(name)
    , m_isClearing(false)
{
    m_managedObjects.reserve(128);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::~AutoreleasePool()
{
    clear();
    PoolManager::getInstance()->pop();
}

void AutoreleasePool::addObject(GameObject* pObject)
{
    m_managedObjects.emplace_back(pObject);
}

void AutoreleasePool::clear()
{
    m_isClearing = true;

    std::vector<GameObject*> releaseings;
    releaseings.swap(m_managedObjects);
    for (const auto& object : releaseings) {
        object->release();
    }

    m_isClearing = false;
}

bool AutoreleasePool::contains(GameObject* pObject) const
{
    for (const auto& object : m_managedObjects) {
        if (object == pObject) {
            return true;
        }
    }
    return false;
}

void AutoreleasePool::dump()
{
    OCFLOG("autorelease pool: %s, number of managed object %d\n", m_name.c_str(),
            static_cast<int>(m_managedObjects.size()));
    OCFLOG("%20s%20s%20s", "Object pointer", "Object id", "reference count");
    for (const auto& object : m_managedObjects) {
        OCFLOG("%20p%20u\n", object, object->getReferenceCount());
    }
}

PoolManager* PoolManager::s_sheredPoolManager = nullptr;

PoolManager* PoolManager::getInstance()
{
    if (s_sheredPoolManager == nullptr) {
        s_sheredPoolManager = new PoolManager();
        new AutoreleasePool("ocf autorelease pool");
    }
    return s_sheredPoolManager;
}

void PoolManager::destroyInstance()
{
    delete s_sheredPoolManager;
    s_sheredPoolManager = nullptr;
}

AutoreleasePool* PoolManager::getCurrentPool() const
{
    return m_releasePoolStack.back();
}

bool PoolManager::isObjectInPools(GameObject* pObject) const
{
    for (const auto& pool : m_releasePoolStack) {
        if (pool->contains(pObject)) {
            return true;
        }
    }
    return false;
}

PoolManager::PoolManager()
{
    m_releasePoolStack.reserve(10);
}

PoolManager::~PoolManager()
{
    while (!m_releasePoolStack.empty()) {
        AutoreleasePool* pool = m_releasePoolStack.back();
        delete pool;
    }
}

void PoolManager::push(AutoreleasePool* pool)
{
    m_releasePoolStack.emplace_back(pool);
}

void PoolManager::pop()
{
    OCFASSERT(!m_releasePoolStack.empty(), "");
    m_releasePoolStack.pop_back();
}


} // namespace ocf
