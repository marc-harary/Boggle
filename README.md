# Boggle

The `Boggle` program solves the classic [Boggle](https://www.google.com/search?q=boggle+board&sxsrf=ALeKk00vViKApghkaR8w8xQJtFuRGlZy1A:1600660650382&source=lnms&tbm=isch&sa=X&ved=2ahUKEwiV8-LLrfnrAhUXoHIEHdsMDMoQ_AUoAXoECB8QAw&biw=1440&bih=821#imgrc=xrG0tUc70oTmuM) board by using a depth-first graph search algorithm.

The program reads a dictionary in the form of a textfile from `stdin` (`words.txt` is provided as an example). It reads the board passed in as a linear string with the number of rows and columns specified as command-line arguments. Thus, the usage is as follows:

`Boggle [-c] [-t] nRows nCols board`

where the `-c` flag outputs 
