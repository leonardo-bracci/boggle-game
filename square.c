#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "square.h"


void generateGrid(){

    // Letters in each dice
    char* dice[] = {
        "rifobx", "ifehey", "denows", "utoknd", "hmsrao", "lupets",
        "acitoa", "ylgkue", "qbmjoa", "ehispn", "vetign", "baliyt",
        "ezavnt", "ralesc", "uwilrg", "pacemd"
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
    if (debugMode == 't'){  
        printf("Dice array correctly shuffled\n");
    }
    
    // rand() % LENGTH;
    char grid[SIDE][SIDE][2];
    int diceCount = 0;

    printf("+---+---+---+---+\n");


    // 2 nested loops to populate the 2D array
    for (int i = 0; i < SIDE; i++){
        printf("|");
        for(int j = 0; j < SIDE; j++){
            // Pick a random letter from each dice
            char letter = dice[diceCount++][rand() % 5];
            grid[i][j][0] = letter;
            // Print the letter to the terminal to be visible for the player
            printf(" %c |", letter);
        }
        printf("\n+---+---+---+---+\n");
       
    }

    if (debugMode == 't'){  
        printf("Grid printed coorectly\n");
    }
}