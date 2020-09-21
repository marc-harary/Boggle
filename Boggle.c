#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "Trie.h"

#define ASCII_START 97
#define ALPHA_SIZE 26
#define quit(mess) fprintf(stderr, mess); exit(0)
#define USAGE "Boggle [-c] [-t] nRows nCols board\n"
#define isCFlag(arg) !strcmp(arg,"-c")
#define isTFlag(arg) !strcmp(arg,"-t")
#define isOKFlag(arg) (isCFlag(arg) || isTFlag(arg))
#define isWildCard(word) (word[strlen(word)-1] == '_')

bool check(char *line);
int *getChdn(int index);
int graphSearch(char *word, int index, bool *visited);

Trie DICT;
bool C = false, T = false;
char *BOARD, **FOUND;
int MAXLEN, HEIGHT, WIDTH, N_FOUND = 0;

int main(int argc, char **argv)
{
    // parse flags
    if (argc == 6) {
        // make sure flags are -c and -t
        if (!isCFlag(argv[1]) || !isTFlag(argv[2])) {
            quit(USAGE);
        } else {
            C = T = true;
        }
    } else if (argc == 5) {
        if (isOKFlag(argv[1])) {
            C = isCFlag(argv[1]);
            T = !C;
        } else {
            quit(USAGE);
        }
    } else if (argc != 4) {
        quit(USAGE);
    }

    // read in height
    char *ptr;
    HEIGHT = strtol(argv[argc-3], &ptr, 10);
    if (HEIGHT < 1 || strcmp(ptr, "")) { // can't have alphas
        quit("HEIGHT MUST BE INT > 0\n");
    }

    // read in width
    ptr = NULL;
    WIDTH = strtol(argv[argc-2], &ptr, 10);
    if (HEIGHT < 1 || strcmp(ptr, "")) {
        quit("WIDTH MUST BE INT > 0\n");
    }

    // get BOARD, making sure it's valid
    BOARD = argv[argc-1];
    for (int i = 0; i < strlen(BOARD); i++) {
        if ( !isalpha(BOARD[i]) && BOARD[i] != '_') {
            quit("BOARD MUST CONSIST OF LETTERS\n");
        } else if (isalpha(BOARD[i])) {
            BOARD[i] = tolower(BOARD[i]);
        }
    }
    if (strlen(BOARD) != HEIGHT*WIDTH) {
        quit("BOARD =! HEIGHT*WIDTH\n");
    }

    // create dictionary trie
    DICT = createTr(0);
    char *line = NULL;
    size_t worldLength = 0;
    while (getline(&line, &worldLength, stdin) >= 0) {
        if (check(line)) {
            line[strlen(line)-1] = '\0';
            insertTr(DICT, line);
            MAXLEN = (strlen(line) > MAXLEN) ?
                strlen(line) : MAXLEN;
        }
    }
    free(line);

    // perform graph search using each node as a start
    char *start = malloc(2);
    bool *visited = calloc(HEIGHT*WIDTH, sizeof(bool));
    for (int i = 0; i < HEIGHT*WIDTH; i++) {
        for (int j = 0; j < HEIGHT*WIDTH; j++) {
            visited[j] = false;
        }
        visited[i] = true;
        // account for _ as initial
        if (BOARD[i] == '_') {
            char sub;
            for (int j = 0; j < ALPHA_SIZE; j++) {
                sub = j+ASCII_START;
                sprintf(start, "%c", sub);
                isWord(DICT, start);
                graphSearch(start, i, visited);
            }
        } else { // is non-wildcard
            sprintf(start, "%c", BOARD[i]);
            isWord(DICT, start); // acount for one-letter words
            graphSearch(start, i, visited);
        }
    }
    free(start);
    free(visited);
    
    // print and clean up
    walkTr(DICT, "", C);
    destroyTr(DICT);
}




/* checks line from dictionary to make sure word contains
only lowercase alphas */
bool check(char *line)
{
    int length = strlen(line)-1;

    for (int i = 0; i < length; i++) {
        if ( !isalpha(line[i]) ) {
            return false;
        } else {
            line[i] = tolower(line[i]);
        }
    }

    return true;
}


/* returns octuple of indices into which 
a path can expand into given a present index */
int *getChdn(int index)
{
    // see if index is on an edge
    int *chdn = calloc(8, sizeof(int)),
        column = index % WIDTH,
        leftEdge = !column, // is on left edge of board, etc.
        rightEdge = (column == WIDTH-1),
        botEdge = (index <= WIDTH-1), // is on bottom edge, etc.
        topEdge = (WIDTH*(HEIGHT-1) <= index
            && index <= HEIGHT*WIDTH-1);

    // left
    chdn[0] = (leftEdge) ? -1 : index-1; 
    // right
    chdn[1] = (rightEdge) ? -1 : index+1; 
    // down
    chdn[2] = (botEdge) ? -1 : index-WIDTH;
    // up
    chdn[3] = (topEdge) ? -1 : index+WIDTH;
    // upper left
    chdn[4] = (leftEdge || topEdge) ? -1 : index+WIDTH-1;
    // upper right
    chdn[5] = (rightEdge || topEdge) ? -1 : index+WIDTH+1;
    // lower left
    chdn[6] = (leftEdge || botEdge) ? -1 : index-WIDTH-1;
    // lower right
    chdn[7] = (rightEdge || botEdge) ? -1 : index-WIDTH+1;

    return chdn;
}


/* recursively performs depth-first graph search, using
backtracking to account for the t-flag */
int graphSearch(char *word, int index, bool *visited)
{
    // account for wildcards
    if (isWildCard(word)) {
        char *subWord = strdup(word);
        visited[index] = true;
        for (int i = 0; i < ALPHA_SIZE; i++) {
            subWord[strlen(word)-1] = i+ASCII_START;
            if (isPrefix(DICT, subWord)) {
                isWord(DICT, subWord);
                graphSearch(subWord, index, visited);
            }
        }
        visited[index] = false; // backtrack
        free(subWord);
        return 1;
    }

    // get next chars to expand into
    int nextIndex, *chdn = getChdn(index);
    char *nextWord = NULL;

    // iterate over children to recurse
    for (int i = 0; i < 8; i++) {
        // if child exists, expand path into it
        if ( (nextIndex=chdn[i]) != -1 ) {
            // skip if -t flag is specified, continue
            if (T && visited[nextIndex]) {
                continue;
            }

            // get next word
            nextWord = malloc(strlen(word) + 2);
            sprintf(nextWord, "%s%c", word, BOARD[nextIndex]);

            // if next word is wildcard, restart
            if (isWildCard(nextWord)) {
                graphSearch(nextWord, nextIndex, visited);
            // if word is valid prefix, recurse
            } else if ( isPrefix(DICT, nextWord) ) {
                isWord(DICT, nextWord);
                visited[nextIndex] = true;
                graphSearch(nextWord, nextIndex, visited);
                visited[nextIndex] = false; // backtrack
            }
            
            free(nextWord);
        }
    }

    free(chdn);
    return 1;
}
