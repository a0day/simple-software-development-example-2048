#include "board.h"
#include <SDL3/SDL_main.h>

using namespace std;

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

    Board board;
    if(!board.init()){
        return 1;
    }

    Nums nums;
    
    bool running = true;
    bool game_over = false;
    bool game_win = false;

    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
            else if(event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat && !game_over){
                char dir = 0;
                switch(event.key.key){
                    case SDLK_UP:    dir = 'w'; break;
                    case SDLK_DOWN:  dir = 's'; break;
                    case SDLK_LEFT:  dir = 'a'; break;
                    case SDLK_RIGHT: dir = 'd'; break; 
                }
                if(dir && nums.move(dir)){
                    nums.spawn();
                    if(nums.isWin()){game_over=true;game_win = true;}
                    if(nums.isLose()){game_over=true;game_win = false;}
                }
            }
        }
        board.draw(nums);
        if(game_over){
            board.drawMessage(game_win);
        }
        board.present();
    }

    board.close();
    return 0;
}