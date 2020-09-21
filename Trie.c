#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "Trie.h"

#define ASCII_START 97
#define ALPHA_SIZE 26

struct trie {
    Trie *kids;
    int c, nFound;
    bool isLeaf, isPrefix;
};

Trie createTr(int c)
{
    Trie new = malloc(sizeof(struct trie));

    // make children
    Trie *kids = calloc(ALPHA_SIZE, sizeof(Trie));
    for (int i = 0; i < ALPHA_SIZE; i++) {
        kids[i] = NULL;
    }

    new->kids = kids;
    new->c = c;
    new->nFound = 0;
    new->isLeaf = false;
    new->isPrefix = true;

    return new;
}

bool insertTr(Trie parent, char *word)
{
    if (!*word) { // base case--word is NULL
        parent->isLeaf = true;
        return true;
    } 

    // get child
    Trie *kid = &parent->kids[*word-ASCII_START];
    if (!*kid) *kid = createTr(*word); // make if nec

    return insertTr(*kid, ++word); // recurse
}

bool destroyTr(Trie parent)
{
    if (!parent) return false;

    for (int i = 0; i < ALPHA_SIZE; i++) {
        destroyTr(parent->kids[i]);
    }

    free(parent->kids);
    free(parent);

    return true;
}

bool isWord(Trie parent, char *word)
{
    if (!parent) {
        return false;
    } else if (!*word) {
        if (parent->isLeaf) {
            parent->nFound++;
            return true;
        }
        return false;
    } else {
        Trie kid = parent->kids[*word-ASCII_START];
        char *nextWord = word + 1;
        return isWord(kid, nextWord);
    }
}

bool isPrefix(Trie parent, char *word)
{
    if (!parent) {
        return false;
    } else if (!*word) {
        return true;
    } else {
        Trie kid = parent->kids[*word-ASCII_START];
        char *nextWord = word + 1;
        return isPrefix(kid, nextWord);
    }
}

bool walkTr(Trie parent, char *word, bool cFlag)
{
    if (!parent || !parent->isPrefix) {
        return false;
    } 
    
    char *nextWord = malloc(strlen(word)+2);
    sprintf(nextWord, "%s%c", word, parent->c);

    if (parent->isLeaf) {
        if (!cFlag && parent->nFound > 0) {
            printf("%s: %d\n", nextWord, parent->nFound);
        }
        else if (cFlag && parent->nFound == 0) {
            printf("%s\n", nextWord);
        }  
    }

    for (int i = 0; i < ALPHA_SIZE; i++) {
        walkTr(parent->kids[i], nextWord, cFlag);
    }

    free(nextWord);
    return true;
}
