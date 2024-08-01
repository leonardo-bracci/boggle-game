
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// Maximum length for a word
#define SIDE 4
#define LENGTH (SIDE * SIDE)
#define N 17576

// Define the alphabet size
#define ALPHABET_SIZE 26

// Hash node structure
typedef struct hashNode {
    char word[LENGTH + 1];
    struct hashNode *next;
} hashNode;

// Trie node structure
typedef struct trieNode {
    struct trieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
} trieNode;

// Function prototypes

// Function to create a new trie node
trieNode *getNode(void);

// Function to insert a word into the trie
void insert(const char *word);

// Function to search for a word in the trie
int search(const char *word);

// Function to check if a node has any children
int hasChildren(trieNode *node);

// Function to free the entire trie
void freeTrie(trieNode *root);

// Function to scramble the array that represents the order of dice to represent on the 4x4 square
void rand_list();

// Function to load the dictionary into memory
void load();

// bool check(char *words);
void load_boggle_hashtable(char square[SIDE][SIDE][3], int row, int col, int letter_count, int moves_count, char *w_boggle);

// Function to print the trie
void printTrie(trieNode *node, char *prefix, int level);

// Letters in each dice
const char* dice[] = {
    "rifobx", "ifehey", "denows", "utoknd", "hmsrao", "lupets",
    "acitoa", "ylgkue", "qbmjoa", "ehispn", "vetign", "baliyt",
    "ezavnt", "ralesc", "uwilrg", "pacemd"
};

// Order of dice to represent in the 4x4 square
int dice_list[LENGTH];

// Tries
trieNode *root;
hashNode *boggle_table[N/26/26];

// Variable for changing the pattern of neighbouring letters
int start = 0;

// 4D array of neighbouring squares for each letter
int movements[SIDE][SIDE][8][2];




int main() {
    // Generate a random list of numbers from 1 to LENGTH
    rand_list();

    // 2D array of characters
    char square[SIDE][SIDE][3];

    // Populate the square array with letters from the dice array
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            int dice_index = dice_list[i * SIDE + j];
            int letter_index = rand() % 6; // Each die has 6 faces
            square[i][j][0] = dice[dice_index][letter_index];
            square[i][j][1] = '0'; // Initialize the visited flag to '0'
        }
    }

    // Print square
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            printf("%c  ", square[i][j][0]);
        }
        printf("\n");
    }
    
    // Initialize the trie
    root = getNode();

    printf("dionazifascio4\n");

    // Upload vocabulary into memory
    load();

    
    printf("dionazifascio3\n");
    // Generate 4D array of neighbouring squares for each letter
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            int coordinates_count = 0;
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    movements[i][j][coordinates_count][0] = 99;
                    movements[i][j][coordinates_count][1] = 99;
                    coordinates_count++;
                }
            }
        }
    }
    printf("dionazifascio2\n");
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            int coordinates_count = 0;
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    int tempx = i + x;
                    int tempy = j + y;

                    // Exclude invalid letters out of the square and same letter
                    if (tempx < 0 || tempx > (SIDE - 1) || tempy < 0 || tempy > (SIDE - 1) || (tempx == i && tempy == j)) {
                        continue;
                    }
                    movements[i][j][coordinates_count][0] = tempx;
                    movements[i][j][coordinates_count][1] = tempy;
                    coordinates_count++;
                }
            }
        }
    }

    printf("dionazifascio1\n");

    // Words in the square
    char w_boggle[LENGTH + 1];

    // Loop through all letters and for each one call the recursive function
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            // Call function for each letter in the square
            load_boggle_hashtable(square, i, j, 0, 0, w_boggle);
            for (int t = 0; t < SIDE; t++) {
                // Clean the square_copy of the 0/1 references for each new letter of the previous loop
                for (int k = 0; k < SIDE; k++) {
                    square[t][k][1] = '0';
                }
            }
        }
    }
    printf("dionazifascio\n");
    char prefix[LENGTH + 1];
    printTrie(root, prefix, 0);

    freeTrie(root);
    
}

// Scramble the array that represents the order of dice to represent on the 4x4 square
void rand_list() {
    // Seed the rand() function with a random number related to time
    srand(time(NULL));

    for (int i = 0; i < LENGTH; i++) { // fill array
        dice_list[i] = i;
    }

    for (int i = 0; i < LENGTH; i++) { // shuffle array
        int temp = dice_list[i];
        int randomIndex = rand() % LENGTH;

        dice_list[i] = dice_list[randomIndex];
        dice_list[randomIndex] = temp;
    }
}



// Load words from square to hashtable
void load_boggle_hashtable(char square[SIDE][SIDE][3], int row, int col, int letter_count, int moves_count, char *w_boggle) {
    // Initiate word to add to hashtable
    w_boggle[letter_count] = square[row][col][0];
    w_boggle[letter_count + 1] = '\0';

    // Sets the flag to '1' to remember I've already passed on this letter
    square[row][col][1] = '1';

    trieNode *currentLetter = root;

    for (int i = 0; w_boggle[i] != '\0'; i++) {
        currentLetter = currentLetter->children[w_boggle[i] - 'a'];
    }

    // Loop through all possible paths for each letter, max 8 per letter
    for (moves_count = 0; moves_count < 8; moves_count++) {
        int tempRow = movements[row][col][moves_count][0];
        int tempCol = movements[row][col][moves_count][1];

        if (square[tempRow][tempCol][1] == '1' || (tempRow == 99 && tempCol == 99) || !hasChildren(currentLetter)) {
            continue;
        }

        // Recursion
        load_boggle_hashtable(square, tempRow, tempCol, letter_count + 1, moves_count, w_boggle);
    }
    square[row][col][1] = '0';
    w_boggle[letter_count] = '\0';
}

// Function to search for a word in the trie
int search(const char *word) {
    trieNode *current = root;
    while (*word) {
        int index = *word - 'a';
        if (!current->children[index])
            return 0;
        current = current->children[index];
        word++;
    }
    return current && current->isEndOfWord;
}

// Function to check if a node has any children
int hasChildren(trieNode *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i])
            return 1;
    return 0;
}

