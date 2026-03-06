#include "ecs/CollisionSystem.h"
#include "ecs/Component.h"
#include <SDL.h>
#include <algorithm>
#include <vector>
#include <utility>

// 重点：加 CollisionSystem:: 前缀，变成类的成员函数
std::pair<float, float> CollisionSystem::get_overlap_depth(EntityManager& em, EntityID e1, EntityID e2) {
    auto* pos1 = em.get_component<PositionComponent>(e1);
    auto* col1 = em.get_component<CollisionComponent>(e1);
    auto* pos2 = em.get_component<PositionComponent>(e2);
    auto* col2 = em.get_component<CollisionComponent>(e2);

    if (!pos1 || !col1 || !pos2 || !col2) return std::make_pair(0.0f, 0.0f);

    float e1_half_w = col1->width / 2.0f;
    float e1_half_h = col1->height / 2.0f;
    float e2_half_w = col2->width / 2.0f;
    float e2_half_h = col2->height / 2.0f;

    float e1_center_x = pos1->x + e1_half_w;
    float e1_center_y = pos1->y + e1_half_h;
    float e2_center_x = pos2->x + e2_half_w;
    float e2_center_y = pos2->y + e2_half_h;

    float x_overlap = e1_half_w + e2_half_w - abs(e1_center_x - e2_center_x);
    float y_overlap = e1_half_h + e2_half_h - abs(e1_center_y - e2_center_y);

    return std::make_pair(x_overlap, y_overlap);
}

bool CollisionSystem::check_collision(EntityManager& em, EntityID e1, EntityID e2) {
    auto* pos1 = em.get_component<PositionComponent>(e1);
    auto* col1 = em.get_component<CollisionComponent>(e1);
    auto* pos2 = em.get_component<PositionComponent>(e2);
    auto* col2 = em.get_component<CollisionComponent>(e2);

    if (!pos1 || !col1 || !pos2 || !col2) return false;

    return (pos1->x < pos2->x + col2->width) &&
        (pos1->x + col1->width > pos2->x) &&
        (pos1->y < pos2->y + col2->height) &&
        (pos1->y + col1->height > pos2->y);
}

void CollisionSystem::resolve_collision(EntityManager& em, EntityID e1, EntityID e2) {
    auto* vel1 = em.get_component<VelocityComponent>(e1);
    auto* vel2 = em.get_component<VelocityComponent>(e2);
    auto* pos1 = em.get_component<PositionComponent>(e1);
    auto* pos2 = em.get_component<PositionComponent>(e2);
    auto* col1 = em.get_component<CollisionComponent>(e1);
    auto* col2 = em.get_component<CollisionComponent>(e2);

    if (!vel1 || !vel2 || !pos1 || !pos2 || !col1 || !col2) return;

    // 彻底删掉结构化绑定，改用pair
    std::pair<float, float> overlap = this->get_overlap_depth(em, e1, e2);
    float x_overlap = overlap.first;
    float y_overlap = overlap.second;

    if (x_overlap <= 0 || y_overlap <= 0) return;

    bool is_horizontal_collision = (x_overlap < y_overlap);

    if (is_horizontal_collision) {
        float push_amount = x_overlap / 2.0f;
        if (pos1->x < pos2->x) {
            pos1->x -= push_amount;
            pos2->x += push_amount;
        }
        else {
            pos1->x += push_amount;
            pos2->x -= push_amount;
        }
        std::swap(vel1->dx, vel2->dx);
        vel1->dx *= 0.95f;
        vel2->dx *= 0.95f;
    }
    else {
        float push_amount = y_overlap / 2.0f;
        if (pos1->y < pos2->y) {
            pos1->y -= push_amount;
            pos2->y += push_amount;
        }
        else {
            pos1->y += push_amount;
            pos2->y -= push_amount;
        }
        std::swap(vel1->dy, vel2->dy);
        vel1->dy *= 0.95f;
        vel2->dy *= 0.95f;
    }
}

void CollisionSystem::update(EntityManager& em, float delta_time) {
    const auto& entities = em.get_entities();
    std::vector<EntityID> entity_list;
    for (EntityID e : entities) {
        entity_list.push_back(e);
    }

    for (size_t i = 0; i < entity_list.size(); i++) {
        EntityID e1 = entity_list[i];
        if (!em.has_component<PositionComponent>(e1) ||
            !em.has_component<CollisionComponent>(e1) ||
            !em.has_component<VelocityComponent>(e1)) {
            continue;
        }

        for (size_t j = i + 1; j < entity_list.size(); j++) {
            EntityID e2 = entity_list[j];
            if (!em.has_component<PositionComponent>(e2) ||
                !em.has_component<CollisionComponent>(e2) ||
                !em.has_component<VelocityComponent>(e2)) {
                continue;
            }

            if (this->check_collision(em, e1, e2)) {
                this->resolve_collision(em, e1, e2);
            }
        }
    }
}