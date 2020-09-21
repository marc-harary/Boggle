typedef struct trie *Trie;

Trie createTr(int c);
bool insertTr(Trie parent, char *word);
bool destroyTr(Trie parent);
bool isWord(Trie parent, char *word);
bool isPrefix(Trie parent, char *word);
bool walkTr(Trie parent, char *word, bool cFlag);
