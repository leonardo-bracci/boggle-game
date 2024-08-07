// #include <ctype.h>
// #include <stdio.h>
// #include <string.h>
// #include <strings.h>
// #include <stdlib.h>
// #include <time.h>
// #include <stdbool.h>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Maximum length for a word
#define SIDE 4
#define LENGTH SIDE * SIDE

// Choose number of elements in hashmap
// const unsigned int N = 17602;
#define N 17576
// const unsigned int N = 17576;


// char WORD[LENGTH + 1];


// Letters in each diceeck
const char *dice[] = {"rifobx", "ifehey", "denows",
                "utoknd", "hmsrao", "lupets",
                "acitoa", "ylgkue", "qbmjoa",
                "ehispn", "vetign", "baliyt",
                "ezavnt", "ralesc", "uwilrg",
                "pacemd"};




// Order of dice to represent in the 4x4 square
int dice_list[LENGTH];

// Node in the hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Hash table
node *table[N];
node *boggle_table[N/26/26];


// Variable for changing the pattern of neighbouring letters
int start = 0;

// 4D array of neighbouring squares for each letter
int movements[SIDE][SIDE][8][2];


// Functions prototypes declaration
void rand_list();
void load();
unsigned int hash(const char *words);
bool check(char *words);
void load_boggle_hashtable(char square[SIDE][SIDE][3], int row, int col, int letter_count, int moves_count, char *w_boggle);
bool impossible_sequence(const char* wordazza);


int main()
{
    // Generate a random list of numbers from 1 to LENGTH
    rand_list();

    // 2D array of characters
    char square[SIDE][SIDE][3];

    // char square[][4][3]= {
    //     {"a0", "b0", "c0", "d0"},
    //     {"e0", "f0", "g0", "h0"},
    //     {"i0", "j0", "k0", "l0"},
    //     {"m0", "n0", "o0", "p0"}
    // };

    // Populate the square array with letters from the dice array
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            int dice_index = dice_list[i * SIDE + j];
            int letter_index = rand() % 6; // Each die has 6 faces
            square[i][j][0] = dice[dice_index][letter_index];
            square[i][j][1] = '0'; // Initialize the visited flag to '0'
        }
    }



    // Upload vocabulary into memory
    load();


    // Generate 4D array of neighbouring squares for each letter
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            int coordinates_count = 0;
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    movements[i][j][coordinates_count][0] = 99;
                    movements[i][j][coordinates_count][1] = 99;
                    coordinates_count++;
                }
            }
        }
    }


    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            int coordinates_count = 0;
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int tempx = i + x;
                    int tempy = j + y;

                    // Exclude invalid letters out of the square and smame letter
                    if (tempx < 0 || tempx > (SIDE - 1) || tempy < 0 || tempy > (SIDE - 1) || (tempx == i && tempy == j))
                    {
                        continue;
                    }
                    movements[i][j][coordinates_count][0] = tempx;
                    movements[i][j][coordinates_count][1] = tempy;
                    coordinates_count++;
                }
            }
        }
    }


    // Words in the square
    char w_boggle[LENGTH + 1];

    

    // Loop through all letters and for each one call the recursive function
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            // Call function for each letter in the square
            load_boggle_hashtable(square, i, j, 0, 0, w_boggle);
            for (int t = 0; t < SIDE; t++)
            {
                // Clean the square_copy of the 0/1 references for each new letter of the previous loop
                for (int k = 0; k < SIDE; k++)
                {
                    square[t][k][1] = '0';
                }
            }
        }
    }

    // Print square
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {

            printf("%c  ", square[i][j][0]);
        }
        printf("\n");
    }


}



// Scramble the array that represents the order of dice to represent on the 4x4 square
void rand_list()
{

    // Seed the rand() function with a random number ralated to time
    srand(time(NULL));

    for (int i = 0; i < LENGTH; i++)
    {     // fill array
        dice_list[i] = i;
    }

    for (int i = 0; i < LENGTH; i++)
    {    // shuffle array
        int temp = dice_list[i];
        int randomIndex = rand() % LENGTH;

        dice_list[i] = dice_list[randomIndex];
        dice_list[randomIndex] = temp;
    }
}

// Loads dictionary into memory, returning true if successful, else false
void load()
{
    FILE *dict = fopen("dictionary/boggle_dic.txt", "r");
    if (dict == NULL)
    {
        printf("Could not open dictionary.\n");
        return;
    }
    char buffer[LENGTH + 2];

    //set all hashtable values to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Reads the dictionary untill reach EOF End Of File
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        //buffer[strlen(buffer)-1] = '\0';
        node *w = malloc(sizeof(node));
        if (w == NULL)
        {
            return;
        }
        //copy word from dictionary file into node
        strcpy(w->word, buffer);
        w->next = NULL;

        //verify position in the hash table
        int current_key = hash(w->word);

        //check if this is the first word to insert in the linked list
        if (table[current_key] == NULL)
        {
            table[current_key] = w;
            // printf("first word with this letter is______%s______\n", table[current_key]->word);
        }
        else
        {
            // Check if the first word in the table is the last word of the dictionary
            w->next = table[current_key];
            table[current_key] = w;
        }
    }
    fclose(dict);
}



// Hashes word to a number to search the hashtable
unsigned int hash(const char *word)
{
    // printf("%s\n", words);
//     return tolower(word[0]) - 'a';
    unsigned int result;

    result = (!!word[0]) * (677 * (tolower(word[0]) - 97));
    result += (word[0] && word[1]) * (27 * (tolower(word[1]) - 97));
    result += (word[0] && word[1] && word[2]) * (tolower(word[2]) - 97) + 1;

    return result;


}



// Returns true if word is in dictionary, else false
bool check(char *words)
{
    int index = hash(words);
    node *temp = table[index];

    while (temp != NULL)
    {
        if (strcasecmp(words, temp->word) == 0)

        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}


// Load words from square to hashtable
void load_boggle_hashtable(char square[SIDE][SIDE][3], int row, int col, int letter_count, int moves_count, char *w_boggle)
{
    // Initiate word to add to hashtable
    w_boggle[letter_count] = square[row][col][0];
    w_boggle[letter_count + 1] = '\0';


    // Sets the flag to '1' to remeber I've already passed on this letter
    square[row][col][1] = '1';

    // // Check if the current word is in the dictionary
    // if (check(w_boggle)) {
    //     printf("Found word: %s\n", w_boggle);
    // }


    // Loop through all possible paths for each letter, max 8 per letter
    for (moves_count = 0; moves_count < 8; moves_count++)
    {
        int tempRow = movements[row][col][moves_count][0]; //0
        int tempCol = movements[row][col][moves_count][1]; //1

    

        

        if (square[tempRow][tempCol][1] == '1'|| (tempRow == 99 && tempCol == 99) /*|| impossible_sequence(w_boggle)*/)
        {
            continue;
        }

        // Recursion
        load_boggle_hashtable(square, tempRow, tempCol, letter_count + 1, moves_count, w_boggle);

    }
    square[row][col][1] = '0';
    w_boggle[letter_count] = '\0';


}

// Check if the word contains an impossible sequence of characters
bool impossible_sequence(const char* wordazza)
{
    const char needles[][4] = {"bx", "cj", "cv", "cx", "dx", "fq", "fx", "gq", "gx",
    "hx", "jc", "jf", "jg", "jq", "js", "jv", "jw", "jx", "jz", "kq", "kx", "mx", "px",
    "pz", "qb", "qc", "qd", "qf", "qg", "qh", "qj", "qk", "ql", "qm", "qn", "qp", "qs", "qt",
    "qv", "qw", "qx", "qy", "qz", "sx", "vb", "vf", "vh", "vj", "vm", "vp", "vq", "vt", "vw", "vx",
    "wx", "xj", "xx", "zj", "zq", "zx", "bxq", "cjx", "cvz", "cxq", "dxq", "fqz", "fxj", "gqz", "gxq", "hxq",
    "jqz", "jfz", "jgz", "jqx", "jsy", "jvz", "jwz", "jzx", "jzk", "kqx", "kxz", "mxq", "pxq", "pqz", "qbq", 
    "qcq", "qdz", "qfz", "qgz", "qhz", "qjj", "qjx", "qkx", "qlz", "qmk", "qnv", "qpx", "qsz", "qtz", "qvx",
    "qwz", "qxz", "qyz", "qzz", "sxz", "vbx", "vfz", "vhz", "vjz", "vmz", "vpx", "vqz", "vtx", "vwz", "vxz",
    "wxj", "xjx", "xxz", "zjx", "zqz", "zxq"};

    // Loop through each impossible sequence
    for (int i = 0; i < sizeof(needles) / sizeof(needles[0]); i++)
    {
        // Check if the current sequence is in the word
        if (strstr(wordazza, needles[i]) != NULL)
        {
            return true;
        }
    }
    return false;
}


