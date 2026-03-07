#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// global variable to determine mode of operation, default is single thread
int num_threads = 1;

// defines the layout of the sudoku board in 9x9 array
typedef struct{
    int cell[9][9];
} sudoku;

// printing board per requirements
// sudoku is set as const to indicate read only 
void print_board(const sudoku *board){
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            printf("%d ", board->cell[i][j]);
        }
        printf("\n");
    }
}

// checker functions for board
// sudoku is set as const to indicate read only 
bool check_row(const sudoku *board, int row){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    for (int col = 0; col < 9; col++){
        int num = board->cell[row][col];
        if (num < 1 || num > 9 || seen[num])return false; // invalid number or invalid row
        seen[num] = true; // sets number as used
    }
    return true; // row is valid
}

bool check_col(const sudoku *board, int col){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    for (int row = 0; row < 9; row++){
        int num = board->cell[row][col];
        if (num < 1 || num > 9 || seen[num]) return false; // invalid number or invalid col
        seen[num] = true; // sets number as used
        
    }
    return true; // col is valid
}

bool check_grid(const sudoku *board, int grid){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    // sets up a 3x3 grid starting in top left, moving right, then down
    int start_row = (grid / 3) * 3;
    int start_col = (grid % 3) * 3;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int num = board->cell[start_row + i][start_col + j];
            if (num < 1 || num > 9 || seen[num])return false; // invalid number or invalid grid
            seen[num] = true; // sets number as used
            
        }
    }
    return true; // grid is valid
}
    

void* work(void* param){
      // thread stuff here

}

// loads the board from input.txt, is hardcoded
// change to input_invalid.txt to test invalid solution
int load_board(sudoku *board){
    FILE *file = fopen("input.txt", "r");

    if(file == NULL){
        printf("Error opening file, is the correct input present?\n");
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
    num_threads = atoi(argv[1]);
    bool valid = true;
    time_t begin = time(NULL);

    // loads the board before checks are run
    if (load_board(&test_board) != 0){
        return -1; // couldn't load board 
    }

    printf("BOARD STATE IN input.txt:\n");
    print_board(&test_board);

    if (num_threads == 1){
       
        // all validation can be done in one loop due to single threading
        // will need to break up into multi loops for multithreading
        for (int i = 0; i < 9; i++){
            if (!check_row(&test_board, i) ||
                !check_col(&test_board, i) ||
                !check_grid(&test_board, i)){
                    valid = false;
                    printf("SOLUTION: NO (%f seconds)\n", (double)(time(NULL) - begin));
                    return 0;
                }
        }
        printf("SOLUTION: YES (%f seconds)\n", (double)(time(NULL) - begin));
    }
    return 0;
}