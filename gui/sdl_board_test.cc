#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

    constexpr int WIDTH = 480;
    constexpr int HEIGHT = 480;

    constexpr float gap = 20;
    constexpr float cell = (WIDTH - 5*gap)/4;

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("2048 SDL3 Test", WIDTH, HEIGHT, 0);
    if(!window){
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer){
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 18, 24, 38, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 64, 128, 255, 255);
        for(int i = 0; i < 4 ;++i){
            for(int j=0; j<4; ++j){
                SDL_FRect rect ={(j+1)*gap + j*cell, (i+1)*gap + i*cell, cell, cell};
                SDL_RenderFillRect(renderer,&rect);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}