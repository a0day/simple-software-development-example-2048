#ifndef NUMS_H
#define NUMS_H

#include <random>

class Nums {
public:
    Nums();
    int get(int row, int col) const {return value[row][col];}
    void spawn();
    bool move(unsigned char direction);
    bool isLose();
    bool isWin();

private:
    int value[4][4]{0};

    //for random number generation
    std::mt19937 rng;
    std::uniform_int_distribution<int> xydist;
    std::uniform_int_distribution<int> numdist;

    bool slide(int *t);
    bool merge(int *t);
    bool processLine(int *t);
};

#endif