#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <SDL.h>
#include <iostream>
#include <cstdlib>

class SDLRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    // 构造函数：初始化SDL窗口和渲染器
    SDLRenderer(int width, int height, const char* title);
    // 析构函数：释放资源
    ~SDLRenderer();
    // 获取SDL渲染器指针（给RenderSystem用）
    SDL_Renderer* get_renderer() const;
};

#endif // SDLRENDERER_H