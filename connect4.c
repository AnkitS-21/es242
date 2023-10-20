#include <stdio.h>
#include <stdlib.h>
//in this code when you run it the red player is you and the blue player is the computer
enum {
    EMPTY = 0,
    RED,
    BLUE,
};

typedef char board_t[4][5];
typedef char player_t;

player_t other_player(player_t player) {
    return (player == RED) ? BLUE : RED;
}

int has_won(board_t board, player_t player)
{
    // check for horizontal win
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row][col + 1] == player &&
                board[row][col + 2] == player &&
                board[row][col + 3] == player) {
                return 1;
            }
        }
    }

    // check for vertical win
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 1; row++) {
            if (board[row][col] == player &&
                board[row + 1][col] == player &&
                board[row + 2][col] == player &&
                board[row + 3][col] == player) {
                return 1;
            }
        }
    }

    // check for diagonal (top-left to bottom-right)
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player &&
                board[row + 3][col + 3] == player) {
                return 1;
            }
        }
    }

    // check for diagonal (bottom-left to top-right)
    for (int row = 3; row >= 2; row--) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row - 1][col + 1] == player &&
                board[row - 2][col + 2] == player &&
                board[row - 3][col + 3] == player) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            if (board[row][col] == EMPTY) {
                return 0; // The board is not full
            }
        }
    }
    return 1; // The board is full
}
int column_full(board_t board, int col) {
    return board[0][col] != EMPTY;
} 

typedef struct {
    int col;
    int score;
} move_t;

int find_empty_row(board_t board, int col, player_t player)
{
    for (int row = 3; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = player;
            return row; 
        }
    }
    return -1; 
}

move_t best_move(board_t board, player_t player, int depth) {
    
    move_t bestMove;
    move_t currentMove = {-1, -1};
    int noMoveFound = 1;

    if (depth == 0 || is_full(board)) {
        currentMove.score = 0;
        return currentMove;
    }
     for (int col = 0; col < 5; ++col) {
        if (!column_full(board, col)) {
            int row = find_empty_row(board, col, player);
            board[row][col] = player;
            if (has_won(board, player)) {
                board[row][col] = EMPTY;
                return (move_t){col, 1};
            }
            board[row][col] = EMPTY;
        }
    } 
    for (int col = 0; col < 5; ++col) {
        if (!column_full(board, col)) {
            int row = find_empty_row(board, col, player);
            board[row][col] = player;
            if (is_full(board)) {
                board[row][col] = EMPTY;
                return (move_t){col, 0};
            }
            currentMove = best_move(board, other_player(player), depth - 1);
            board[row][col] = EMPTY;

            if (currentMove.score == -1) {
                return (move_t){col, 1};
            } else if (currentMove.score == 0) {
                bestMove = (move_t){col, 0};
                noMoveFound = 0;
            } else {
                if (noMoveFound) {
                    bestMove = (move_t){col, -1};
                    noMoveFound = 0;
                }
            }
        }
    }

    return bestMove;
}
void print_board(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            switch (board[row][col]) {
                case EMPTY:
                    printf(". ");
                    break;
                case RED:
                    printf("R ");
                    break;
                case BLUE:
                    printf("B ");
                    break;
            }
        }
        printf("\n");
    }
    printf("1 2 3 4 5\n\n");
}

int main() {
    board_t board;
    player_t currentPlayer = RED;  // Start with RED player, red player is the human player

    // Initialize the board to all empty spaces
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            board[i][j] = EMPTY;
        }
    }

    while (1) {
        print_board(board);

        if (currentPlayer == RED) {
            // Human player's turn
            int col;
            printf("%s's turn. Enter a column (1-5): ", (currentPlayer == RED) ? "RED" : "BLUE");
            scanf("%d", &col);

            // Ensure the column input is valid
            if (col < 1 || col > 5) {
                printf("Invalid column. Please choose a column between 1 and 5.\n");
                continue;
            }

            col--; // Adjust the column to the zero-based index

            // Check if the selected column is full
            if (column_full(board, col)) {
                printf("Column is full. Please choose another column.\n");
                continue;
            }

            // Apply the move to the board
            int emptyRow = find_empty_row(board, col, currentPlayer);
            board[emptyRow][col] = currentPlayer;
        } else {
            // Computer player's turn
            move_t computerMove = best_move(board, currentPlayer, 3);  // You can adjust the depth here
            int col = computerMove.col;

            // Check if the selected column is full
            if (column_full(board, col)) {
                continue;  // Choose another move if the column is full
            }

            // Apply the move to the board
            int emptyRow = find_empty_row(board, col, currentPlayer);
            board[emptyRow][col] = currentPlayer;

            // Check if the computer has won after making the move
            if (has_won(board, currentPlayer)) {
                print_board(board);
                printf("%s player wins!\n", (currentPlayer == RED) ? "RED" : "BLUE");
                break;
            }
        }

        // Check if the current player has won
        if (has_won(board, currentPlayer)) {
            print_board(board);
            printf("%s player wins!\n", (currentPlayer == RED) ? "RED" : "BLUE");
            break;
        }

        // Check if the board is full (a draw)
        if (is_full(board)) {
            printf("It's a draw!\n");
            break;
        }

        // Switch to the other player
        currentPlayer = other_player(currentPlayer);
    }

    return 0;
}
