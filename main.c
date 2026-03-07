#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// global variable to determine mode of operation, default is single thread
int mode_select = 1;
// global variable to check if board is valid
bool valid = true;

// protects valid boolean
pthread_mutex_t lock;

// defines the layout of the sudoku board in 9x9 array
typedef struct{
    int cell[9][9];
} sudoku;

typedef struct{
    sudoku s;
    int counter;
} params;

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

void* check_row_t(void* param){
    params* f = (params*)param;
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    for (int col = 0; col < 9; col++){
        int num = (f->s).cell[f->counter][col];
        if (num < 1 || num > 9 || seen[num]){
            pthread_mutex_lock(&lock);
            valid = false; // invalid number or invalid grid
            pthread_mutex_unlock(&lock);
            return NULL; // stop checking as soon as error is found
        } // invalid number or invalid row
        seen[num] = true; // sets number as used
    }
    return NULL;
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

void* check_col_t(void* param){
    params* f = (params*)param;
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    for (int row = 0; row < 9; row++){
        int num = (f->s).cell[row][f->counter];
        if (num < 1 || num > 9 || seen[num]) {
            pthread_mutex_lock(&lock);
            valid = false; // invalid number or invalid grid
            pthread_mutex_unlock(&lock);
            return NULL; // stop checking as soon as error is found
        } // invalid number or invalid col
        seen[num] = true; // sets number as used
    } // col is valid
    return NULL;
}

bool check_grid(const sudoku *board, int grid){
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    // sets up a 3x3 grid starting in top left, moving right, then down
    int start_row = (grid / 3) * 3;
    int start_col = (grid % 3) * 3;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int num = board->cell[start_row + i][start_col + j];
            if (num < 1 || num > 9 || seen[num]){return false;} // invalid number or invalid grid
            seen[num] = true; // sets number as used
        }
    }
    return true; // grid is valid
}

void* check_grid_t(void* param){
    params* f = (params*)param;
    bool seen[10] = {false}; // check index 1-9 for repeated numbers
    // sets up a 3x3 grid starting in top left, moving right, then down
    int start_row = (f->counter / 3) * 3;
    int start_col = (f->counter % 3) * 3;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            int num = (f->s).cell[start_row + i][start_col + j];
            if (num < 1 || num > 9 || seen[num]){
                pthread_mutex_lock(&lock);
                valid = false; // invalid number or invalid grid
                pthread_mutex_unlock(&lock);
                return NULL; // stop checking as soon as error is found
            }
             seen[num] = true; // sets number as used
        }
    }
    return NULL;
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
    mode_select = atoi(argv[1]);
    time_t begin = time(NULL);

    // loads the board before checks are run
    if (load_board(&test_board) != 0){
        return -1; // couldn't load board 
    }

    printf("BOARD STATE IN input.txt:\n");
    print_board(&test_board);


    if (mode_select == 1){
       
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

    }else{
        params arg[27];
        pthread_mutex_init(&lock, NULL);
        // multithread mode goes here
        pthread_t tid[27]; // array to track thread ids

        for(int i = 0; i < 9; ++i) // check rows using 9 threads
        {
            arg[i].s = test_board;
            arg[i].counter = i;
            pthread_create(&(tid[i]), NULL, check_row_t, &arg[i]);
        }

        for(int i = 9; i < 18; ++i) // check columns using 9 threads
        {
            arg[i].s = test_board;
            arg[i].counter = i - 9;
            pthread_create(&(tid[i]), NULL, check_col_t, &arg[i]);
        }

        for(int i = 18; i < 27; ++i) // check grids using 9 threads
        {
            arg[i].s = test_board;
            arg[i].counter = i - 18;
            pthread_create(&(tid[i]), NULL, check_grid_t, &arg[i]);
        }

        for(int i = 0; i < 27; ++i)
        {
            pthread_join(tid[i], NULL);
        }

        pthread_mutex_destroy(&lock);

        printf("SOLUTION: %s", valid ? "YES" : "NO");
        printf(" (%f seconds)\n", (double)(time(NULL) - begin));
        fflush(stdout);
    }

    return 0;
}
