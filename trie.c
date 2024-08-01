#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "trie.h"


// Generate a new trie node
trieNode* generateTrieNode() {
    // Allocate memory for new node
    trieNode* node = malloc(sizeof(trieNode));
    // Set flag to "not end of word"
    node->isEndOfWord = 0;
    // Set all children to NULL
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

// Load dictionary into memory
void load_dictionary(trieNode* root) {
    FILE *dictionary = fopen("dictionary/dictionary.txt", "r");
    if (dictionary == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    char buffer[LENGTH + 2];
    while (fscanf(dictionary, "%s", buffer) == 1) {
        char word[LENGTH + 1];
        int j = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                word[j++] = buffer[i];
            }
        }
        word[j] = '\0';
        addWordToTrie(root, word);
    }
    fclose(dictionary);
}

// Add word to trie
void addWordToTrie(trieNode* root, char* word) {
    // Create a temporary node to traverse the trie, the pointer points at the same memory location as the root
    // is not a copy of the root, it is the root and therefore the trie itself
    trieNode* tempNode = root;
    // Traverse the trie, if the node does not exist, create it
    // Checks one letter at the time of the word until the end of the word
    for (int i = 0; word[i] != '\0'; i++) {
        // Uses ASCII values to determine the index of the children array
        int index = word[i] - 'a';
        if (tempNode->children[index] == NULL) {
            tempNode->children[index] = generateTrieNode();
        }
        tempNode = tempNode->children[index];
    }
    tempNode->isEndOfWord = 1;
}

// Print entire trie to terminal
void printTrie(trieNode* root, char* word, int level) {
    if (root->isEndOfWord) {
        word[level] = '\0';
        printf("%s\n", word);
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            word[level] = i + 'a';
            printTrie(root->children[i], word, level + 1);
        }
    }
}

// Free trie
void freeTrie(trieNode* root) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            freeTrie(root->children[i]);
        }
    }
    free(root);
}


