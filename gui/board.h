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

    static constexpr SDL_Color bgColor = {250, 248, 239, 255};
    static constexpr SDL_Color cellColor = {205, 193, 180, 255};

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    SDL_Texture* digit_tex[11] = {};
    float tex_w[11] = {};
    float tex_h[11] = {};
    SDL_Color bg[11] = {{238, 228, 218, 255}, {237, 224, 200, 255}, {242, 177, 121, 255}, {245, 149, 99, 255}, {246, 124, 95, 255}, {246, 94, 59, 255}, {237, 207, 114, 255}, {237, 204, 97, 255}, {237, 200, 80, 255}, {237, 197, 63, 255}, {237, 194, 46, 255}};
    SDL_Color fg[11] = {{119, 110, 101, 255}, {119, 110, 101, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}, {249, 246, 242, 255}};

    SDL_Texture* mes_tex[2] = {};
    float mes_w[2] = {};
    float mes_h[2] = {};
};

#endif