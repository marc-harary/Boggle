# Boggle

The `Boggle` program solves the classic [Boggle](https://www.google.com/search?q=boggle+board&sxsrf=ALeKk00vViKApghkaR8w8xQJtFuRGlZy1A:1600660650382&source=lnms&tbm=isch&sa=X&ved=2ahUKEwiV8-LLrfnrAhUXoHIEHdsMDMoQ_AUoAXoECB8QAw&biw=1440&bih=821#imgrc=xrG0tUc70oTmuM), printing out all words that makeable via the board that are also listed within a dictionary passed in via `stdin`.

The usage is as follows:

`Boggle [-c] [-t] nRows nCols board`

where using the `-c` flag prints out all words in the dictionary that are not found in the board and using `-t` enables the program to use the same letters multiple times.

The program uses a depth-first graph search algorithm to walk to the board and stores the word from the dictionary in a trie.
