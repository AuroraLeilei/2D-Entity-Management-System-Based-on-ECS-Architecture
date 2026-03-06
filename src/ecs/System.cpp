#include "ecs/System.h"
#include "ecs/Component.h"
#include <SDL.h>

// ========== MovementSystem 实现 ==========
void MovementSystem::update(EntityManager& em, float delta_time) {
    // 1. 修正方法名：get_entities() 而不是 get_active_entities()
    const auto& entities = em.get_entities();

    // 2. 遍历unordered_set要用迭代器（不能直接for-each）
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        EntityID entity = *it;

        // 3. 先检查组件是否存在，避免空指针
        if (!em.has_component<PositionComponent>(entity) ||
            !em.has_component<VelocityComponent>(entity)) {
            continue;
        }

        // 4. 用auto*接收指针（不再用auto&）
        auto* pos = em.get_component<PositionComponent>(entity);
        auto* vel = em.get_component<VelocityComponent>(entity);

        // 5. 指针访问成员用 -> 而不是 .
        pos->x += vel->dx * delta_time;
        pos->y += vel->dy * delta_time;

        // 边界碰撞反弹（窗口边缘）
        if (pos->x < 0 || pos->x > 800 - 20) vel->dx *= -1;
        if (pos->y < 0 || pos->y > 600 - 20) vel->dy *= -1;
    }
}

// ========== RenderSystem 实现 ==========
// 注意：删除重复的构造函数实现（System.h里已经写了）
void RenderSystem::update(EntityManager& em, float delta_time) {
    // 1. 清空渲染器
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // 2. 修正方法名+遍历方式
    const auto& entities = em.get_entities();
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        EntityID entity = *it;

        // 3. 检查组件
        if (!em.has_component<PositionComponent>(entity) ||
            !em.has_component<RenderComponent>(entity)) {
            continue;
        }

        // 4. auto* 接收指针
        auto* pos = em.get_component<PositionComponent>(entity);
        auto* render = em.get_component<RenderComponent>(entity);

        // 5. 绘制矩形（指针用 ->）
        SDL_Rect rect = {
            static_cast<int>(pos->x),
            static_cast<int>(pos->y),
            static_cast<int>(render->width),
            static_cast<int>(render->height)
        };

        // 设置矩形颜色（指针用 ->）
        SDL_SetRenderDrawColor(
            renderer,
            render->color.r,
            render->color.g,
            render->color.b,
            render->color.a
        );
        SDL_RenderFillRect(renderer, &rect);
    }

    // 刷新渲染器（显示绘制内容）
    SDL_RenderPresent(renderer);
}