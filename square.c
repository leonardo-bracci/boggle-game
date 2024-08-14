#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "square.h"


// void generateGrid (char[SIDE][SIDE][2]){
void generateGrid(char grid[SIDE][SIDE][2]){

    // Letters in each dice
    char* dice[] = {
    "aaeegn", "elrtty", "aaoott", "abbjoo", "ehrrtv", "cimotu",
    "distty", "eiosst", "delrvy", "achops", "himnqu", "eeinsu",
    "eeghnw", "affkps", "hlnnrz", "deilrx"
    };

    
    // Seed the rand() function with a random number related to time
    srand(time(NULL));

    // Shuffle de dice with the Fisher-Yates algorithm
    for (int i = LENGTH - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = dice[i];
        dice[i] = dice[j];
        dice[j] = temp;
        }

        
    // // Print the dice array to check if it was shuffled
    // for (int i = 0; i < LENGTH; i++){
    //     printf("%s\n", dice[i]);
    // }
    
    // 2 nested loops to populate the 2D array
    int diceCount = 0;
    for (int i = 0; i < SIDE; i++){
        for(int j = 0; j < SIDE; j++){
            // Pick a random letter from each dice
            char letter = dice[diceCount++][rand() % 5];
            grid[i][j][0] = letter;

            // Set visited flag to false ('f') for the recursion
            grid[i][j][1] = 'f';
        }
    }
}

// Print the grid to the terminal
void printGrid(char grid[SIDE][SIDE][2]){

    printf("+---+---+---+---+\n");


    // 2 nested loops to populate the 2D array
    for (int i = 0; i < SIDE; i++){
        printf("|");
        for(int j = 0; j < SIDE; j++){
            printf(" %c |", grid[i][j][0]);
        }
        printf("\n+---+---+---+---+\n");
       
    }

    if (debugMode == 't'){  
        printf("Grid printed coorectly\n");
    }
}

