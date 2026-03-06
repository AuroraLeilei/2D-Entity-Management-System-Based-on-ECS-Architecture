#define SDL_MAIN_HANDLED

#include "ecs/EntityManager.h"
#include "ecs/System.h"
#include "renderer/SDLRenderer.h"
#include "ecs/CollisionSystem.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    // 初始化随机数
    srand(time(nullptr));

    // 初始化SDL渲染器
    SDLRenderer sdl_renderer(800, 600, "ECS 2D Entity Manager");

    // 初始化ECS
    EntityManager em;
    std::vector<std::unique_ptr<System>> systems;

    // 添加系统
    systems.push_back(std::make_unique<MovementSystem>());
    systems.push_back(std::make_unique<CollisionSystem>());
    systems.push_back(std::make_unique<RenderSystem>(sdl_renderer.get_renderer()));

    // 创建10个实体
    const int entity_count = 20;
    SDL_Color colors[] = {
        {255,0,0,255}, {0,255,0,255}, {0,0,255,255},
        {255,255,0,255}, {255,0,255,255}, {0,255,255,255},
        {180,150,120,255}, {120,150,180,255}, {120,170,140,255}, {170,120,180,255}
    };

    for (int i = 0; i < entity_count; i++) {
        EntityID e = em.create_entity();
        em.add_component<PositionComponent>(e, rand() % 780, rand() % 580);
        em.add_component<VelocityComponent>(e, (rand() % 100 - 50) / 1.0f, (rand() % 100 - 50) / 1.0f);
        em.add_component<RenderComponent>(e, 20, 20, colors[i]);
        em.add_component<CollisionComponent>(e, 20, 20);
    }

    // 主循环
    bool running = true;
    Uint32 last_time = SDL_GetTicks();
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // 计算delta_time
        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        // 更新所有系统
        for (auto& system : systems) {
            system->update(em, delta_time);
        }

        SDL_Delay(16);
    }

    return 0;
}