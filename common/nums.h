#ifndef NUMS_H
#define NUMS_H

#include <random>

class Nums {
public:
    Nums();
    int get(int row, int col) const {return value[row][col];}
    int getScore() const {return score;}

    void spawn();
    bool move(unsigned char direction);
    bool isLose();
    bool isWin();

    void reset(){
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                value[i][j] = 0;
            }
        }
        score = 0;
        spawn();
        spawn();
    }

private:
    int value[4][4]{0};
    int score;

    //for random number generation
    std::mt19937 rng;
    std::uniform_int_distribution<int> xydist;
    std::uniform_int_distribution<int> numdist;

    bool slide(int *t);
    bool merge(int *t);
    bool processLine(int *t);
};

#endif