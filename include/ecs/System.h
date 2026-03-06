#ifndef SYSTEM_H
#define SYSTEM_H

#include "EntityManager.h"
#include <memory>
#include <SDL.h> // 新增：引用SDL头文件
class CollisionSystem; // 前向声明

// 系统基类
class System {
public:
    virtual ~System() = default;
    virtual void update(EntityManager& em, float delta_time) = 0;
};

// 移动系统
class MovementSystem : public System {
public:
    void update(EntityManager& em, float delta_time) override;
};

// 渲染系统
class RenderSystem : public System {
private:
    SDL_Renderer* renderer;
public:
    // 构造函数：只在这里实现一次，System.cpp里不再重复
    RenderSystem(SDL_Renderer* ren) : renderer(ren) {}
    void update(EntityManager& em, float delta_time) override;
};

#endif // SYSTEM_H