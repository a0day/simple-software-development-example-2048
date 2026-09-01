#include "board.h"

bool Board::init(){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("2048 game", WIDTH, HEIGHT, 0);
    if(!window){
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer){
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if(!TTF_Init()){
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    const char* base = SDL_GetBasePath();
    std::string font_path = std::string(base) + "arial.ttf";

    font = TTF_OpenFont(font_path.c_str(),40);
    if(!font){
        SDL_Log("font loaded failed: %s", SDL_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    //预渲染11个数字
    for(int i = 0, val=2; i<11; ++i, val*=2){
        std::string s = std::to_string(val); 

        SDL_Surface* surface = TTF_RenderText_Blended(font, s.c_str(), s.size(), fg[i]);
        if(!surface){
            SDL_Log("surface create failed: %s", SDL_GetError());
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;   
        }

        digit_tex[i] = SDL_CreateTextureFromSurface(renderer, surface);
        if(!digit_tex[i]){
            SDL_Log("texture create failed: %s", SDL_GetError());
            for(int j=0;j<i;++j){
                SDL_DestroyTexture(digit_tex[j]);
            }
            SDL_DestroySurface(surface);
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false; 
        }
        SDL_GetTextureSize(digit_tex[i], &tex_w[i], &tex_h[i]);

        SDL_DestroySurface(surface);
    }

    //预渲染两个消息
    std::string messages[2] = {"Game Over!","You Win!"};
    for(int i=0; i<2; ++i){
        SDL_Surface* surface = TTF_RenderText_Blended(font, messages[i].c_str(), messages[i].size(), {255,255,255,255});
        if(!surface){
            SDL_Log("surface create failed: %s", SDL_GetError());
            for(int j=0;j<11;++j){
                SDL_DestroyTexture(digit_tex[j]);
            }
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;   
        }

        mes_tex[i] = SDL_CreateTextureFromSurface(renderer, surface);
        if(!mes_tex[i]){
            SDL_Log("texture create failed: %s", SDL_GetError());
            for(int j=0;j<11;++j){
                SDL_DestroyTexture(digit_tex[j]);
            }
            for(int j=0;j<i;++j){
                SDL_DestroyTexture(mes_tex[j]);
            }
            SDL_DestroySurface(surface);
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false; 
        }
        SDL_GetTextureSize(mes_tex[i], &mes_w[i], &mes_h[i]);

        SDL_DestroySurface(surface);
    }

    return true;
}

//use only when the program init sucessfully, otherwise it will crash
void Board::close(){
    for(int i=0; i<2; ++i)
        SDL_DestroyTexture(mes_tex[i]);
    for(int i=0;i<11;++i){
        SDL_DestroyTexture(digit_tex[i]);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Board::draw(const Nums& nums){
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(renderer);



    for(int row = 0; row < 4 ;++row){
        for(int col=0; col<4; ++col){
            float tx = (col+1)*gap + col*cell;
            float ty = (row+1)*gap + row*cell;

            int val = nums.get(row,col);
            int exp = 0;
            for(int v=val; v>1; v>>=1) ++exp;

            SDL_Color color = (val==0) ? cellColor : bg[exp-1];
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_FRect rect ={tx, ty, cell, cell};
            SDL_RenderFillRect(renderer,&rect);
            
            if(val == 0) continue;

            SDL_Texture* texture = digit_tex[exp - 1];
            float tw = tex_w[exp - 1];
            float th = tex_h[exp - 1];

            SDL_FRect dst = { tx + (cell-tw)/2, ty + (cell-th)/2, tw, th };
            SDL_RenderTexture(renderer, texture, nullptr, &dst);

        }
    }   
}


void Board::drawMessage(bool win){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);   // alpha=150 半透明
    SDL_FRect overlay = { 0, 0, WIDTH, HEIGHT };
    SDL_RenderFillRect(renderer, &overlay);

    SDL_Texture* texture = mes_tex[win];
    float tw = mes_w[win];
    float th = mes_h[win];

    SDL_FRect dst = { (WIDTH-tw)/2, (HEIGHT-th)/2, tw, th };
    SDL_RenderTexture(renderer, texture, nullptr, &dst);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); 
}