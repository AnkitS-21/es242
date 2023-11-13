#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include<tuple>
#include<cmath>
#include<queue>
#include<assert.h>

using namespace std;



enum {
    X=1,
    O=2
};

struct board{
    int arr[3][3];
};

void print_board(const board& board){
    for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
            cout<<board.arr[r][c]<<" ";
        }
        cout<<endl;
    }
    return;
}

pair<int,int> find_space(const board& board){
    int r=0,c=0;
    for(r=0;r<3;r++){
        for(c=0;c<3;c++){
            if(board.arr[r][c]==0){
                return make_pair(r,c);
            }
        }
    }
    assert(0);
}
bool checkvalid(const board& board ){         
    for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
            if(board.arr[r][c]>1){return false;}
            if((r*c==1)){
                if(board.arr[1][1]!=0){return false;}
            }
        }
    }
    return true;
 
}

int ord(const board& board){
    int o=0;
    int i=0;
    for(int r=0;r<3;r++){
        for(int c=0;c<3;c++){
            o+=board.arr[r][c]*pow(7,i);
            i++;
        }
    }
    return o;
}

board up(board b, int r, int c){
    board bnew=b;
    int val=b.arr[r-1][c];
    if(val==1){val=6;}
    else if(val==2){val=5;}
    else if(val==5){val=1;}
    else if(val==6){val=2;}

    bnew.arr[r][c]=val;
    bnew.arr[r-1][c]=0;
    

    return bnew;
}

board down(const board& b, int r, int c){
    board bnew=b;
    int val=b.arr[r+1][c];
    if(val==1){val=5;}
    else if(val==2){val=6;}
    else if(val==5){val=2;}
    else if(val==6){val=1;}
    bnew.arr[r][c]=val;
    bnew.arr[r+1][c]=0;
    return bnew;
}

board left(const board& b, int r, int c){
    board bnew=b;
    int val=b.arr[r][c-1];
    if(val==1){val=4;}
    else if(val==2){val=3;}
    else if(val==3){val=1;}
    else if(val==4){val=2;}
    bnew.arr[r][c]=val;
    bnew.arr[r][c-1]=0;
    return bnew;
}

board right(const board& b, int r, int c){
    board bnew=b;
    int val=b.arr[r][c+1];
    if(val==1){val=3;}
    else if(val==2){val=4;}
    else if(val==3){val=2;}
    else if(val==4){val=1;}
    bnew.arr[r][c]=val;
    bnew.arr[r][c+1]=0;
    return bnew;
}

