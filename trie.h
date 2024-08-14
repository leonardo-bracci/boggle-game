#ifndef TRIE_H
#define TRIE_H

// DEFINITIONS

// Alphabet size, all letters
#define ALPHABET_SIZE 26
// // Side of the sqauare
#define SIDE 4
// Maximum length of a word because there are 16 letters on the grid
#define LENGTH (SIDE * SIDE)

// Trie node structure
typedef struct trieNode {
    int isEndOfWord;
    struct trieNode *children[ALPHABET_SIZE];    
} trieNode;

// FUNCTIONS DECLARATIONS
// Generate trie node
trieNode* generateTrieNode();
// Load dictionary into memory
void load_dictionary(trieNode* root);
// Add word to trie
void addWordToTrie(trieNode* root, char* word);
// Print trie
void printTrie(trieNode* root, char* word, int level);
// Check if a word is in the trie
int isWordInTrie(trieNode* root, char* word);
// Check if a node has children
int hasChildren(trieNode* node);
// Check if the last letter of a word has children
int hasLastLetterChildren(trieNode* root, char* word);
// Free trie
void freeTrie(trieNode* root);



#endif
