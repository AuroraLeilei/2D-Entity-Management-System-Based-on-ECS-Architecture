#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "Component.h" // 继承IComponent

// 碰撞组件：纯数据，记录碰撞盒的宽高（复用渲染组件的宽高也可以，这里单独定义更灵活）
struct CollisionComponent : public IComponent {
    float width;  // 碰撞盒宽度
    float height; // 碰撞盒高度

    // 构造函数：默认用20x20的碰撞盒（和渲染矩形一致）
    CollisionComponent(float w = 20.0f, float h = 20.0f) : width(w), height(h) {}
};

#endif // COLLISIONCOMPONENT_H