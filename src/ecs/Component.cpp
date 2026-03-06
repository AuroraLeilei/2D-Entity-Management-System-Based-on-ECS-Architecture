#include "ecs/Component.h"

// 初始化组件类型ID的全局变量
uint32_t next_component_id = 1;

// PositionComponent 构造函数实现
PositionComponent::PositionComponent(float x_, float y_) : x(x_), y(y_) {}

// VelocityComponent 构造函数实现
VelocityComponent::VelocityComponent(float dx_, float dy_) : dx(dx_), dy(dy_) {}

// RenderComponent 构造函数实现
RenderComponent::RenderComponent(int w, int h, SDL_Color c) : width(w), height(h), color(c) {}