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
    Down=1,
    Up=2
};

struct board{
    int arr[3][3];
};

void printBoard(const board& board){
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
bool checkValidmoves(const board& board ){         
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
    int value=b.arr[r-1][c];
    if(value==1){value=6;}
    else if(value==2){value=5;}
    else if(value==5){value=1;}
    else if(value==6){value=2;}

    bnew.arr[r][c]=value;
    bnew.arr[r-1][c]=0;
    

    return bnew;
}

board down(const board& b, int r, int c){
    board bnew=b;
    int value=b.arr[r+1][c];
    if(value==1){value=5;}
    else if(value==2){value=6;}
    else if(value==5){value=2;}
    else if(value==6){value=1;}
    bnew.arr[r][c]=value;
    bnew.arr[r+1][c]=0;
    return bnew;
}

board left(const board& b, int r, int c){
    board bnew=b;
    int value=b.arr[r][c-1];
    if(value==1){value=4;}
    else if(value==2){value=3;}
    else if(value==3){value=1;}
    else if(value==4){value=2;}
    bnew.arr[r][c]=value;
    bnew.arr[r][c-1]=0;
    return bnew;
}

board right(const board& b, int r, int c){
    board bnew=b;
    int value=b.arr[r][c+1];
    if(value==1){value=3;}
    else if(value==2){value=4;}
    else if(value==3){value=2;}
    else if(value==4){value=1;}
    bnew.arr[r][c]=value;
    bnew.arr[r][c+1]=0;
    return bnew;
}

uint8_t visited[40353600]={0};
uint64_t parent[40353600];

vector<char> solve(const board& b){

    int count=0;
    queue<board> q;             
    q.push(b);
    visited[ord(b)] = 'S';        
    while(!q.empty()){
        board u=q.front();
        q.pop();
        if(checkValidmoves(u)){
            vector<char> moves;
            int o=ord(u);
            while(visited[o]!='S'){
                moves.push_back(visited[o]);
                o=parent[o];
            }
            std::reverse(moves.begin(), moves.end());
            printBoard(u);
            return moves;
        }   

        auto p=find_space(u);
        int row= p.first;
        int col= p.second;
        // cout<<row<<col<<endl;
        board a,b,c,d;
        if(row!=0){
            a=up(u,row,col);              
            int aord = ord(a);
            if (!visited[aord]) {
                visited[aord] = 'U';
                parent[aord] = ord(u);
                q.push(a);count++;
            }
        }
        if(row!=2){
            b = down(u,row,col);  
            int bord = ord(b);
            if (!visited[bord]) {
                visited[bord] = 'D';
                parent[bord] = ord(u);
                q.push(b);count++;
            }
        }
        if(col!=0){
            c = left(u,row,col);
            int cord = ord(c);
            if (!visited[cord]) {
                visited[cord] = 'L';
                parent[cord] = ord(u);
                q.push(c);count++;
            }
        }
        if(col!=2){
            d = right(u,row,col);
            int dord = ord(d);
            if (!visited[dord]) {
                visited[dord] = 'R';
                parent[dord] = ord(u);
                q.push(d);count++;
            }
        }
    }
    vector<char> v;
    v.push_back(0);
    return v;
}

void read_board(board& b){                               
    for(int r=0;r<3;r++){                              
        for(int c=0;c<3;c++){
            if(r*c==1){
                b.arr[r][c]=0;
            }
            else{
                int value=2;
               
                assert(value!=0);
                b.arr[r][c]=value;
            }
        }
    }
    return;
}

int main(){
    board b;
    read_board(b);
    printBoard(b);
   
    auto moves=solve(b);
   
    if(moves[0]==0){
        cout<<"The board is unsolvable";
    }
    for(char c: moves){
        cout<<c<<endl;
    }
    return 0;
}
