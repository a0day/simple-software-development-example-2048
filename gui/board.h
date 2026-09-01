#ifndef board_h
#define board_h

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "nums.h"
#include <string>

class Board{
public:
    bool init();
    void close();

    void draw(const Nums& nums);
    void drawMessage(bool win);
    void present(){SDL_RenderPresent(renderer);}

private:
    static constexpr int WIDTH = 480;
    static constexpr int HEIGHT = 480;

    static constexpr float gap = 20;
    static constexpr float cell = (WIDTH - 5*gap)/4;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    SDL_Texture* digit_tex[11] = {};
    float tex_w[11] = {};
    float tex_h[11] = {};

    SDL_Texture* mes_tex[2] = {};
    float mes_w[2] = {};
    float mes_h[2] = {};
};

#endif