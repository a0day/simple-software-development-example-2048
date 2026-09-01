#include "nums.h"

//only spawn a new number when the player has made a valid move
void Nums::spawn(){
    int x, y;
    do{
        x = xydist(rng);
        y = xydist(rng);
    }while(value[x][y] != 0);
    value[x][y] = (numdist(rng) == 9 ? 4 : 2);
}

Nums::Nums() : rng(std::random_device{}()), xydist(0, 3), numdist(0, 9) {
    spawn();
    spawn();
    score = 0;
}

bool Nums::move(unsigned char direction){
    switch(direction){
        case 'W': case 'w':{
            int t[4]; 
            bool flag = false;
            for(int i=0; i<4; ++i){
                for(int j=0; j<4; ++j)
                    t[j] = value[j][i];
                if(processLine(t))
                    flag = true;
                for(int j=0; j<4; ++j)
                    value[j][i] = t[j];
            }
            return flag;
        }
        case 'A': case 'a':{
            bool flag = false;
            for(int i=0; i<4; ++i){
                if(processLine(value[i]))
                    flag = true;
            }
            return flag;
        }
        case 'S': case 's':{
            bool flag = false;
            int t[4];
            for(int i=0; i<4; ++i){
                for(int j=0; j<4; ++j)
                    t[j] = value[3-j][i];
                if(processLine(t))
                    flag = true;
                for(int j=0; j<4; ++j)
                    value[3-j][i] = t[j];
            }
            return flag;
        }
        case 'D' : case 'd' :{
            bool flag = false;
            int t[4];
            for(int i=0; i<4; ++i){
                for(int j=0; j<4; ++j)
                    t[j] = value[i][3-j];
                if(processLine(t))
                    flag = true;
                for(int j=0; j<4; ++j)
                    value[i][3-j] = t[j];
            }
            return flag;
        }
        default:
            return false;
    }
}

bool Nums::processLine(int *t){
    bool f1 = slide(t);
    bool f2 = merge(t);
    bool f3 = slide(t);
    return (f1 || f2 || f3);
}

bool Nums::slide(int *t){
    bool flag = false;
    for(int i=1; i<4; ++i){
        if(t[i] == 0) continue;
        int tmp = i;
        while((tmp-1) >= 0 && t[tmp-1] == 0)
            --tmp;
        if(tmp != i){
            t[tmp] = t[i];
            t[i] = 0;
            flag = true;
        }
    }  
    return flag;     
}
//use it after use slide, otherwise it will not work
bool Nums::merge(int *t){
    bool flag = false;
    int i = 0;
    while(i<3 && t[i] != 0){
        if(t[i] == t[i+1]){
            t[i] *= 2;
            t[i+1] = 0;
            score += t[i];
            i+=2;
            flag = true;
        }
        else
            ++i;
    }
    return flag;
}

bool Nums::isLose(){
    for(int i=0; i<4; ++i){
        for(int j=0; j<4; ++j){
            if(value[i][j] == 0)
                return false;
            if(i < 3 && value[i][j] == value[i+1][j])
                return false;
            if(j < 3 && value[i][j] == value[i][j+1])
                return false;
        }
    }
    return true;
}

bool Nums::isWin(){
    for(int i = 0; i<4; ++i){
        for(int j=0; j<4; ++j){
            if(value[i][j] == 2048)
                return true;
        }
    }
    return false;
}