#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// global variable to determine mode of operation, default is single thread
int mode_select = 1;

// defines the layout of the sudoku board in 9x9 array
typedef struct{
    int cell[9][9];
} sudoku;


// checker functions for board
bool check_row(sudoku *board, int row){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    for (int col = 0; col < 9; col++){
        int num = board->cell[row][col];
        if (num < 1 || num > 9 || seen[num]){
            return false; // invalid number or invalid row
            seen[num] = true; // sets number as used
        }
    }
    return true; // row is valid
}

bool check_col(sudoku *board, int col){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    for (int row = 0; row < 9; row++){
        int num = board->cell[row][col];
        if (num < 1 || num > 9 || seen[num]){
            return false; // invalid number or invalid col
            seen[num] = true; // sets number as used
        }
    }
    return true; // col is valid
}

bool check_grid(sudoku *board, int grid){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    // sets up a 3x3 grid starting in top left, moving right, then down
    int start_row = (grid / 3) * 3;
    int start_col = (grid % 3) * 3;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int num = board->cell[start_row + i][start_col + j];
            if (num < 1 || num > 9 || seen[num]){
                return false; // invalid number or invalid grid
                seen[num] = true; // sets number as used
            }
        }
    }
    return true; // grid is valid
}
    
    // thread stuff here
void* work(void* param){

}

// loads the board from input.txt, is hardcoded
// change to input_invalid.txt to test invalid solution
int load_board(sudoku *board){
    FILE *file = fopen("input.txt", "r");

    if(file == NULL){
        printf("Error opening file, is input.txt present?\n");
        return -1;
    }

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            fscanf(file, "%d", &board->cell[i][j]);
        }
    }

    fclose(file);
    return 0;
}

// takes argument for mode select to determine threading method
// mode 1: single thread, mode 2: multithread (#of threads TBD)
int main(int argc, char** argv){

    sudoku test_board;
    mode_select = atoi(argv[1]);

    if (mode_select == 1){
        bool valid = true;
        for (int i = 0; i < 9; i++){
            if (!check_row(&test_board, i) ||
                !check_col(&test_board, i) ||
                !check_grid(&test_board, i)){
                    valid = false;
                    break;
                }
        }
        printf("SOLUTION: %s\n", valid ? "YES" : "NO");
    }
    else {
        // multithread mode goes here
    }









}