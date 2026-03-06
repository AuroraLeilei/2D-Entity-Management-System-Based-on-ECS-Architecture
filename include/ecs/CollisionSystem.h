#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

// 第一步：先引 System.h（确保 System 类已定义）
#include "System.h"
// 第二步：引其他依赖
#include "Component.h"
#include "EntityManager.h"
#include <algorithm>
#include <utility> // for pair
#include <vector>


// --------------- 碰撞组件（CollisionComponent）---------------
struct CollisionComponent : public IComponent {
    float width;  // 碰撞盒宽度
    float height; // 碰撞盒高度
    CollisionComponent(float w = 20.0f, float h = 20.0f) : width(w), height(h) {}
};

// --------------- 碰撞系统（CollisionSystem）---------------
class CollisionSystem : public System {
public:
    void update(EntityManager& em, float delta_time) override;

private:
    bool check_collision(EntityManager& em, EntityID e1, EntityID e2);
    void resolve_collision(EntityManager& em, EntityID e1, EntityID e2);
    std::pair<float, float> get_overlap_depth(EntityManager& em, EntityID e1, EntityID e2);
};

#endif // COLLISION_SYSTEM_H