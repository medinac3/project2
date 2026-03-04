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

}

bool check_col(sudoku *board, int col){

}

bool check_grid(sudoku *board, int grid){

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