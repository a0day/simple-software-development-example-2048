#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include "canvas.h"
#include "nums.h"

class Game {
public:
    Canvas canvas;
    Nums nums;

    void draw() {
        canvas.reset();
        //merge nums into canvas
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < 4; ++j){
                int v = nums.get(i, j);
                if(v == 0)
                    continue;
                std::string num_str = std::to_string(v);
                for(size_t k = 0; k < num_str.size(); ++k){
                    canvas.buffer[i*2+1]
                        [j*Canvas::CELL_WIDTH + 1 + (Canvas::CELL_WIDTH-1-num_str.size())/2 + k] 
                            = num_str[k];
                }
            }
        }
        //cout
        for(int i = 0; i < Canvas::HEIGHT; ++i){
            std::cout << canvas.buffer[i] << '\n';
        }
    }
};

#endif