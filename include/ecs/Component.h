#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <SDL.h>

// 组件基类（类型擦除）
struct IComponent {};

// 全局变量声明（用于生成唯一组件ID）
extern uint32_t next_component_id;

// 组件类型ID生成器（模板，修复静态变量定义）
template <typename T>
struct ComponentType {
    static const uint32_t ID; // 声明
};

// 关键：模板静态变量的定义（必须写在头文件里）
template <typename T>
const uint32_t ComponentType<T>::ID = next_component_id++;

// 提前声明组件类（避免模板实例化报错）
struct PositionComponent;
struct VelocityComponent;
struct RenderComponent;

// 显式实例化（告诉编译器生成这三个组件的ID）
template const uint32_t ComponentType<PositionComponent>::ID;
template const uint32_t ComponentType<VelocityComponent>::ID;
template const uint32_t ComponentType<RenderComponent>::ID;

// 具体组件声明
struct PositionComponent : public IComponent {
    float x, y;
    PositionComponent(float x_ = 0.0f, float y_ = 0.0f);
};

struct VelocityComponent : public IComponent {
    float dx, dy;
    VelocityComponent(float dx_ = 0.0f, float dy_ = 0.0f);
};

struct RenderComponent : public IComponent {
    int width, height;
    SDL_Color color;
    RenderComponent(int w = 20, int h = 20, SDL_Color c = { 255,0,0,255 });
};

#endif // COMPONENT_H