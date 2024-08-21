
/*
GAME OF BOGGLE
to compile: gcc -o boggle main.c trie.c square.c
to run: ./boggle

For unix systems (Linux, gitHub codespace, etc){
    to run with debugging info: gcc -g -o boggle main.c trie.c square.c
    to check memory leaks: valgrind --leak-check=full ./boggle
}

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "globals.h"
#include "trie.h"
#include "square.h"

char debugMode = 'f';


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

// Clear the input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Recursion to calculate the possible words
void recursion(trieNode* root, int col, int row, char* word);

// Hashes word to a number to search the hashtable
unsigned int hash(const char *word);

// Hashes word to a number to search the hashtable
unsigned int hash(const char *word);

// Add word to hash table
void addWord(char *word);

// Print the hash table
void printHashTable();

// Returns true if word is in dictionary, else false
char check(char *words);

// Free hash table
void freeHashTable();

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

    // RULES OF THE GAME
    printf("RULES OF THE GAME:\n");
    printf("1. Boggle is a word game played on a 4x4 grid of letters.\n");
    printf("2. The goal of the game is to find as many words as possible by connecting adjacent letters.\n");
    printf("3. Words must be at least 4 letters long.\n");
    printf("4. Letters in a word can be adjacent horizontally, vertically, or diagonally.\n");
    printf("5. Each letter can only be used once in a word.\n");
    printf("6. Proper nouns, abbreviations, and words with hyphens or apostrophes are not allowed.\n");
    printf("7. The game ends when the player reaches a score of 10.\n");
    printf("\n");

    printf("Press enter to continue...\n");
    clearInputBuffer();
    getchar();
    

    // populate 4x4 square of letters
    generateGrid(grid);
    if (debugMode == 't'){
        printf("Grid correctly populated\n");  
    }
    
    // initialize hash table
    for(int i = 0; i < sizeof(boggle_table)/sizeof(boggle_table[0]); i++){
        boggle_table[i] = generateHashNode();
    }


    // calculate all possible words in the square
    char word[LENGTH + 1];
    for (int i = 0; i < SIDE; i++){
        for (int j = 0; j < SIDE; j++){
            word[0] = grid[i][j][0];
            word[1] = '\0';
            recursion(root, i, j, word);
        }
    }

    // Print hash table
    if (debugMode == 't'){
        char yesOrNo;
        printf("Do you want to print the hashtable? (y/n): ");
        fscanf(stdin, " %c", &yesOrNo);
        if (yesOrNo == 'y') {
            printHashTable();
            printf("HashTable printed\n");
        }
    }

    // Print the grid
    printGrid(grid);

    // LOGIC OF THE GAME
    int score = 0;
    while(score <  10){
        // Ask the user to input a word
        char userWord[LENGTH + 1];
        printf("Enter a word: ");
        fscanf(stdin, "%15s", userWord);
        // Check if the word is in the hash table
        if (check(userWord) == 't') {
            printf("The word is in the hash table\n");
            score++;
            printf("Score: %i\n", score);
        } else {
            printf("The word is not in the hash table\n");
        }
    }

    // Free trie
    freeTrie(root);
    if (debugMode == 't'){
        printf("All memory of trie freed\n");
    }

    // Free hash table
    void freeHashTable();
    if(debugMode == 't'){
        printf("All memory of hash table freed\n");
    }

    return 0;
}


void recursion(trieNode* root, int col, int row, char* word) {

    // Array of offsets to check all 8 neighbors
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
        int newCol = col + offsets[k][0];
        int newRow = row + offsets[k][1];

        // Check if the neighbor is within bounds and hasn't been visited
        if (newCol < 0 || newCol >= SIDE || newRow < 0 || newRow >= SIDE || grid[newCol][newRow][1] == 't' || !hasLastLetterChildren(root, word))
            continue;

        // Now that we know we can extend the word, set the visited flag
        grid[newCol][newRow][1] = 't';

        // Append the letter to the word
        int len = strlen(word);
        word[len] = grid[newCol][newRow][0];
        word[len + 1] = '\0';

        

        // Check if the word is in the dictionary
        if (strlen(word) > 3 && isWordInTrie(root, word) == 1 && check(word) == 'f') {
            // Add word to hashtable if it's valid and not already in there
            addWord(word);
        }

        // Recursive call to explore further
        recursion(root, newCol, newRow, word);

        // Backtrack: remove the last letter and reset the visited flag
        word[len] = '\0';
        grid[newCol][newRow][1] = 'f';
    }
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
    // Hash the word
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
unsigned int hash(const char *word){

    // unsigned int result;

    // printf("%s\n", words);
    return tolower(word[0]) - 'a';

    // result = (!!word[0]) * (677 * (tolower(word[0]) - 97));
    // result += (word[0] && word[1]) * (27 * (tolower(word[1]) - 97));
    // result += (word[0] && word[1] && word[2]) * (tolower(word[2]) - 97) + 1;

    // return result;
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

// Print the hash table
void printHashTable() {
    for (int i = 0; i < 26; i++) {
        hashNode *current = boggle_table[i];
        while (current != NULL) {
            printf("%s\n", current->word);
            current = current->next;
        }
    }
}


void freeHashTable() {
    for (int i = 0; i < 26; i++) {
        hashNode *current = boggle_table[i];
        while (current != NULL) {
            hashNode *temp = current;
            current = current->next;
            free(temp);
        }
        boggle_table[i] = NULL;
    }
}

