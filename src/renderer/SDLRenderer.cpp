#include "renderer/SDLRenderer.h"
#include <iostream>

SDLRenderer::SDLRenderer(int width, int height, const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL³õÊ¼»¯Ê§°Ü£º" << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "äÖÈ¾Æ÷´´½¨Ê§°Ü£º" << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

SDLRenderer::~SDLRenderer() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Renderer* SDLRenderer::get_renderer() const {
    return renderer;
}