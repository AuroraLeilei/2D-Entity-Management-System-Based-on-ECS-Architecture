#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Component.h"
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <typeindex>

// 实体ID类型定义
using EntityID = uint32_t;

class EntityManager {
private:
    // 下一个要分配的实体ID
    EntityID next_entity_id = 1;
    // 所有活跃实体的集合
    std::unordered_set<EntityID> active_entities;
    // 实体 -> 组件类型ID -> 组件智能指针（核心存储结构）
    std::unordered_map<EntityID, std::unordered_map<uint32_t, std::unique_ptr<IComponent>>> entity_components;

public:
    // ========== 公有方法声明（必须和实现一致） ==========
    // 1. 创建实体（返回唯一ID）
    EntityID create_entity();

    // 2. 销毁实体（移除ID和所有组件）
    void destroy_entity(EntityID entity);

    // 3. 添加组件（返回组件引用，方便直接修改）
    template <typename T, typename... Args>
    T& add_component(EntityID entity, Args&&... args);

    // 4. 获取组件（返回指针，无组件则返回nullptr）
    template <typename T>
    T* get_component(EntityID entity); // 声明为 T*，不是 T&

    // 5. 检查实体是否有指定组件
    template <typename T>
    bool has_component(EntityID entity);

    // 6. 获取所有活跃实体（只读）
    const std::unordered_set<EntityID>& get_entities() const {
        return active_entities;
    }
};

// ========== 模板方法实现 ==========
// 创建实体
inline EntityID EntityManager::create_entity() {
    EntityID id = next_entity_id++;
    active_entities.insert(id);
    return id;
}

// 销毁实体
inline void EntityManager::destroy_entity(EntityID entity) {
    active_entities.erase(entity);
    entity_components.erase(entity);
}

// 添加组件（模板实现）
template <typename T, typename... Args>
T& EntityManager::add_component(EntityID entity, Args&&... args) {
    // 创建组件智能指针
    auto comp = std::make_unique<T>(std::forward<Args>(args)...);
    T& comp_ref = *comp; // 保存组件引用
    // 存入组件映射表（用ComponentType<T>::ID作为key）
    entity_components[entity][ComponentType<T>::ID] = std::move(comp);
    return comp_ref;
}

// 获取组件（模板实现，返回指针）
template <typename T>
T* EntityManager::get_component(EntityID entity) {
    // 1. 检查实体是否存在
    if (entity_components.find(entity) == entity_components.end()) {
        return nullptr;
    }
    // 2. 检查组件是否存在
    auto& comp_map = entity_components[entity];
    auto comp_it = comp_map.find(ComponentType<T>::ID);
    if (comp_it == comp_map.end()) {
        return nullptr;
    }
    // 3. 类型转换：IComponent* -> T*
    return static_cast<T*>(comp_it->second.get());
}

// 检查组件是否存在
template <typename T>
bool EntityManager::has_component(EntityID entity) {
    if (entity_components.find(entity) == entity_components.end()) {
        return false;
    }
    return entity_components[entity].count(ComponentType<T>::ID) > 0;
}

#endif // ENTITYMANAGER_H