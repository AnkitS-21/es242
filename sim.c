#include <stdio.h>
#include <assert.h>

#include "test.h"

enum {
    NO = 0,
    RED = 1,
    BLUE = 2
};

typedef char board_t[15];
typedef char player_t;

void init_board(board_t board){
    for (int i = 0; i < 15; i++) {
        board[i] = NO;
    }
}

player_t other(player_t player){
    if (player==RED){
        return BLUE;
    }
    else if (player==BLUE){
        return RED;
    } 
}

int has_won(board_t board, player_t player) {
    int a[4] = {4, 7, 9, 10};
    int b[3] = {3, 5, 6};
    int c[2] = {2, 3};
    int d = 1;
    
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + a[i]] == other(player)) {
                return 1;
            }
        }
    }

    for (int i = 5; i < 8; i++) {
        for (int j = i + 1; j <= 8; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + b[i - 5]] == other(player)) {
                return 1;
            }
        }
    }

    for (int i = 9; i < 11; i++) {
        for (int j = i + 1; j <= 11; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + c[i - 9]] == other(player)) {
                return 1;
            }
        }
    }

    for (int i = 12; i < 13; i++) {
        for (int j = i + 1; j <= 13; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + d] == other(player)) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board){
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    int line;
    int score;
} move_t;

move_t best_move(board_t board, player_t player){
    move_t ans; move_t temp; int nt = 1;
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            board[i] = player;
            if (has_won(board, player)) {
                board[i] = NO;
                return (move_t){i, 1};
            }
            board[i] = NO;
        }
    }
    player_t opponent = other(player);
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            board[i] = opponent;
            if (has_won(board, opponent)) {
                board[i] = NO; 
                return (move_t){i, 1};
            }
            board[i] = NO;
        }
    }
    for (int i = 0; i < 15; i++) {
        if (board[i] == NO) {
            board[i] = player;
            ans = best_move(board, other(player));
            board[i] = NO;
            if (is_full(board)) {
                board[i] = NO;
                move_t y;
                y.line = i; y.score = 0;
                return y;
            }
            if (ans.score == -1) {
                return (move_t){i, 1};
            } else if (ans.score == 0) {
                temp = (move_t){i, 0};
                nt = 0;
            } else {
                if (nt) {
                    temp = (move_t){i, -1};
                    nt = 0;
                }
            }
        }
    }

    return temp;
}

void print_board(board_t board){
    for (int i = 0; i < 15; i++) {
        switch (board[i]) {
            case NO: printf("0  "); break;
            case RED: printf("R  "); break;
            case BLUE: printf("B  "); break;
        }
    }
    printf("\n");
}

int main(){
    int move, i;
    board_t board;
    move_t ans;
    int x;
    printf("You are red, if you want to move first enter 1 else enter 2: ");
    scanf("%d", &x);
    player_t now;
    if (x == 1) {
        now = RED;
    } else if (x == 2) {
        now = BLUE;
    } else {
        printf("\n Wrong Choice");
        return 1; 
    }
    init_board(board);
    while (1) {
        print_board(board);
        printf("\n");
        if (now == RED) {
            printf("0  1  2  3  4  5  6  7  8  9 10 11 12 13 14\n");
            printf("\nEnter move: ");
            scanf("%d", &move);
            i = move;
            if (i >= 0 && i < 15 && board[i] == NO) {
                board[i] = now;
            } 
            else {
                printf("Invalid move. Try again\n");
                continue;
            }
        } else {
            ans = best_move(board, now);
            board[ans.line] = now;
        }

        if (has_won(board, now)){
            print_board(board);
            if (now == RED) {
                printf("R has won!\n");
            } 
            else {
                printf("B has won!\n");
            }
            break;
        }
        now = other(now);
    }
    return 0;
}
