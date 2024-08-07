
/*
GAME OF BOGGLE
to compile: gcc -o boggle main.c trie.c square.c
to run: ./boggle
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "globals.h"
#include "trie.h"
#include "square.h"

char debugMode = 't';


// DEFINITIONS
// Hash node structure
    typedef struct hashNode {
        char word[LENGTH + 1];
        struct hashNode *next;
    } hashNode;


// initialize hash table
hashNode *boggle_table[26];

// Declare the grid of letters
char grid[SIDE][SIDE][2];

hashNode* generateHashNode();

//FUNCTIONS DECLARATIONS

// Recursion to calculate the possible words
void recursion(trieNode* root, int col, int row, char* word);

// Hashes word to a number to search the hashtable
unsigned int hash(char *word);

// Returns true if word is in dictionary, else false
char check(char *words);

// Add word to hash table
void addWord(char *word);

int main() {
    // Initialize trie
    trieNode* root = generateTrieNode();
    if (debugMode == 't'){
        printf("Trie initialized\n");
    }

    // Load dictionary into memory
    load_dictionary(root);
    if (debugMode == 't'){
        printf("Dictionary loaded into memory\n");
    }
    // Print trie
    if (debugMode == 't'){
        char yesOrNo;
        printf("Do you want to print the trie? (y/n): ");
        fscanf(stdin, " %c", &yesOrNo);
        if (yesOrNo == 'y') {
            char word[LENGTH + 1];
            printTrie(root, word, 0);
            if (debugMode == 't'){
                printf("Trie printed\n");
            }
        }
    }

    // populate 4x4 square of letters
    generateGrid(grid);
    if (debugMode == 't'){
        printf("Grid correctly populated \n");  
    }
    
    // initialize hash table
    for(int i = 0; i < sizeof(boggle_table)/sizeof(boggle_table[0]); i++){
        boggle_table[i] = generateHashNode();
    }


    // calculate all possible words in the square
    char word[LENGTH + 1] = "";
    for (int i = 0; i < SIDE; i++){
        for (int j = 0; j < SIDE; j++){
            recursion(root, i, j, word);
        }
    }



    // Free trie
    freeTrie(root);
    if (debugMode == 't'){
        printf("All memory of trie freed\n");
    }

    return 0;
}


void recursion(trieNode* root, int col, int row, char* word) {
    // Current location in the grid
    // if(debugMode == 't'){
    //     printf("current cell = grid[%i][%i]: %c\n",col, row, grid[col][row][0]);
    // }
    // print word
    // if(debugMode == 't' && strlen(word) < 4){
    //     printf("Word: %s\n", word);
    // }
    // base case to exit the recursion
    if(grid[col][row][2] == 't'){
        return;
    }  
    // set visited flag to true 
    grid[col][row][1] = 't';
    
    // array of offsets to check all 8 neighbors
    int offsets[8][2] = {
        {-1, -1}, // Top-left
        {-1, 0},  // Top
        {-1, 1},  // Top-right
        {0, -1},  // Left
        {0, 1},   // Right
        {1, -1},  // Bottom-left
        {1, 0},   // Bottom
        {1, 1}    // Bottom-right
    };

    // For loop to traverse all the coordinates
    for (int k = 0; k < 8; k++) {
        int ni = col + offsets[k][0];
        int nj = row + offsets[k][1];

        // Check if the neighbor is within bounds
        if (ni >= 0 && ni < SIDE && nj >= 0 && nj < SIDE && grid[ni][nj][1] == 'f') {
            grid[ni][nj][1] = 't';
            // Append the letter to the word
            word[strlen(word)] = grid[ni][nj][0];
            word[strlen(word)] = '\0';
            // Check if the word is in the dictionary
            if (isWordInTrie(root, word) == 1) {
                // Check if the word is in the hash table
                if (check(word) == 'f'){
                    // Add the word to the hash table
                    addWord(word);
                    printf("%s\n", word);
                }
            }
            recursion(root, ni, nj, word);
        }
        
    }
    // After recursive call, reset the visited flag
        word[strlen(word) - 1] = '\0';
        grid[col][row][1] = 'f';

}

// Generate a new hash node
hashNode* generateHashNode(){
    // allocate memory for a new node
    hashNode* node = malloc(sizeof(hashNode));
    node->next = NULL;
    node->word[0] = '\0';

    return node;

}


void addWord(char *word) {
    // Get the hash index
    unsigned int hashIndex = hash(word);
    // Create a new node
    hashNode* newNode = generateHashNode();
    
    // Copy the word into the new node
    strcpy(newNode->word, word);

    // Insert the node into the hash table
    if (boggle_table[hashIndex] == NULL) {
        // No collision, simply insert
        boggle_table[hashIndex] = newNode;
    } else {
        // Collision, insert at the beginning of the list
        newNode->next = boggle_table[hashIndex];
        boggle_table[hashIndex] = newNode;
    }
}


// Hashes word to a number to search the hashtable
unsigned int hash(char *word){
    // printf("%s\n", words);
//     return tolower(word[0]) - 'a';
    unsigned int result;

    result = (!!word[0]) * (677 * (tolower(word[0]) - 97));
    result += (word[0] && word[1]) * (27 * (tolower(word[1]) - 97));
    result += (word[0] && word[1] && word[2]) * (tolower(word[2]) - 97) + 1;

    return result;
}

// Returns true if word is in dictionary, else false
char check(char *words){
    int index = hash(words);

    hashNode *temp = boggle_table[index];

    while (temp != NULL)
    {
        if (strcasecmp(words, temp->word) == 0){
            return 't';
        }
        temp = temp->next;
    }
    return 'f';
}