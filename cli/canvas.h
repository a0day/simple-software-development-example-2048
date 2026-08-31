#ifndef CANVAS_H
#define CANVAS_H

class Canvas {
public:
    static constexpr int WIDTH = 21;
    static constexpr int HEIGHT = 9;

    static constexpr int CELL_WIDTH = 5;

    char buffer[HEIGHT][WIDTH+1];

    void reset(){
        for(int i = 0; i < HEIGHT; ++i){
            buffer[i][WIDTH] = '\0';
            if(i % 2 == 0)
                for(int j = 0; j < WIDTH; ++j)
                    buffer[i][j] = '-';
            else{
                for(int j = 0; j < WIDTH; ++j){
                    if(j % CELL_WIDTH == 0)
                        buffer[i][j] = '|';
                    else
                        buffer[i][j] = ' ';
                }
            }
        }
    }

    Canvas(){
        reset();
    }
};

#endif