Sudoku Solver
-------------

This automated sudoku puzzle solver will begin by asking the user to input the name of the file containing a sudoku grid to be solved. This file, such as **"example\_puzzle.txt"** should contain nothing but a sudoku board in the following format:

```
000801000
000000430
500000000
000070800
000000100
020030000
600000075
003400000
000200600
				  
```

The program **"sudoku.cpp"** will solve the given sudoku puzzle, using various techniques. The minimum number of hints that a solveable sudoku board may begin with is 17, and this program has no trouble solving those boards in significantly under a second.

Once the program has determined the solution, it will report both the initial board and the final board as follows:

```
Initial Board: 

+-------+-------+-------+
| 0 0 0 | 8 0 1 | 0 0 0 | 
| 0 0 0 | 0 0 0 | 4 3 0 | 
| 5 0 0 | 0 0 0 | 0 0 0 | 
+-------+-------+-------+
| 0 0 0 | 0 7 0 | 8 0 0 | 
| 0 0 0 | 0 0 0 | 1 0 0 | 
| 0 2 0 | 0 3 0 | 0 0 0 | 
+-------+-------+-------+
| 6 0 0 | 0 0 0 | 0 7 5 | 
| 0 0 3 | 4 0 0 | 0 0 0 | 
| 0 0 0 | 2 0 0 | 6 0 0 | 
+-------+-------+-------+


Final Board: 

+-------+-------+-------+
| 3 6 9 | 8 4 1 | 7 5 2 | 
| 2 8 1 | 7 9 5 | 4 3 6 | 
| 5 7 4 | 3 2 6 | 9 8 1 | 
+-------+-------+-------+
| 1 3 6 | 5 7 4 | 8 2 9 | 
| 7 9 5 | 6 8 2 | 1 4 3 | 
| 4 2 8 | 1 3 9 | 5 6 7 | 
+-------+-------+-------+
| 6 4 2 | 9 1 8 | 3 7 5 | 
| 9 5 3 | 4 6 7 | 2 1 8 | 
| 8 1 7 | 2 5 3 | 6 9 4 | 
+-------+-------+-------+
```
