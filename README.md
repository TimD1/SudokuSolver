Sudoku Solver
-------------

This automated sudoku puzzle solver will begin by asking the user to input the name of the file containing a sudoku grid to be solved. This file, such as **"example\_puzzle.txt"** should contain nothing but a sudoku board in the following format:

```
200804006
006000500
074000920
300040007
000305000
400060009
019000740
008000200
500608001
```

The program **"sudoku.cpp"** will attempt to solve the given sudoku puzzle, using various techniques. Although the program cannot yet solve every possible sudoku puzzle, that is my ultimate goal. Currently, however, it has solved all tested puzzles beginning with more than 22 hints, and has solved several puzzles with fewer.

Once the program can either make no more progress or is finished, it will report both the initial board and the final board as follows:

```
Initial Board: 

+-------+-------+-------+
| 2 0 0 | 8 0 4 | 0 0 6 | 
| 0 0 6 | 0 0 0 | 5 0 0 | 
| 0 7 4 | 0 0 0 | 9 2 0 | 
+-------+-------+-------+
| 3 0 0 | 0 4 0 | 0 0 7 | 
| 0 0 0 | 3 0 5 | 0 0 0 | 
| 4 0 0 | 0 6 0 | 0 0 9 | 
+-------+-------+-------+
| 0 1 9 | 0 0 0 | 7 4 0 | 
| 0 0 8 | 0 0 0 | 2 0 0 | 
| 5 0 0 | 6 0 8 | 0 0 1 | 
+-------+-------+-------+


Final Board: 

+-------+-------+-------+
| 2 5 3 | 8 9 4 | 1 7 6 | 
| 1 9 6 | 2 3 7 | 5 8 4 | 
| 8 7 4 | 1 5 6 | 9 2 3 | 
+-------+-------+-------+
| 3 8 1 | 9 4 2 | 6 5 7 | 
| 9 6 7 | 3 8 5 | 4 1 2 | 
| 4 2 5 | 7 6 1 | 8 3 9 | 
+-------+-------+-------+
| 6 1 9 | 5 2 3 | 7 4 8 | 
| 7 3 8 | 4 1 9 | 2 6 5 | 
| 5 4 2 | 6 7 8 | 3 9 1 | 
+-------+-------+-------+

```
