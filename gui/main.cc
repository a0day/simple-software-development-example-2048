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
    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = false;
            }
        }
        board.draw(nums);
    }

    board.close();
    return 0;
}