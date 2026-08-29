#include "game.h"

using namespace std;

int main(){
    cout<<"welcome to 2048 game!"<<endl;
    while(true){
        cout<<"want a new game? (y/n): ";
        string inp;
        if(!getline(cin, inp)) break;   
        
        if(inp.length()!=1 || (inp[0]!='y' && inp[0]!='n')){
            cout<<"invalid input"<<endl;
            continue;
        }

        if(inp[0]=='n'){
            cout<<"bye!"<<endl;
            break;
        }
        if(inp[0]=='y'){
            Game game;
            game.draw();
            while(true){
                cout<<"input direction (WASD or q): ";
                string input;
                getline(cin, input);
                if(input.length()!=1 || 
                (input[0]!='w' && input[0]!='a' && input[0]!='s' && input[0]!='d' 
                && input[0]!='W' && input[0]!='A' && input[0]!='S' && input[0]!='D' && input[0]!='q' && input[0]!='Q')){
                    cout<<"invalid input"<<endl;
                    game.draw();
                    continue;
                }

                if(input[0]=='q' || input[0]=='Q'){
                    cout<<"bye!"<<endl;
                    break;
                }

                if(game.nums.move(input[0]))
                    game.nums.spawn(); 
                
                game.draw();

                if(game.nums.isWin()){
                    cout<<"you win!"<<endl;
                    break;
                }
                if(game.nums.isLose()){
                    cout<<"you lose!"<<endl;
                    break;
                }
            }
        }
    }

    return 0;
}