
/*
GAME OF BOGGLE
to compile: gcc -o boggle main.c trie.c square.c
to run: ./boggle
*/


#include <stdio.h>
#include "globals.h"
#include "trie.h"
#include "square.h"

char debugMode = 'f';


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

    // generate 4x4 square of letters
    generateGrid();
    
     


    // Free trie
    freeTrie(root);
    if (debugMode == 't'){
        printf("All memory of trie freed\n");
    }

    return 0;
}
