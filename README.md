Sudoku Solver
-------------
This sudoku solver is unique from most others because my original objective was writing a program that could solve any sudoku puzzle without making any guesses or backtracking. Even after implementing several fairly advanced sudoku techniques and convoluted logic, I was unable to achieve this objective for the most difficult of puzzles. It is possible that certain puzzles cannot be solved solely through the use of logic, and additionally require several correct lucky guesses to be made.

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

The program **"sudoku.cpp"** will solve any given sudoku puzzle by using various techniques. The runtime of the program varies greatly, ranging from under a millisecond for the easiest of puzzles to several hundredths of a second for the most challenging puzzles, as seen in the example below. For puzzles of lesser and moderate difficulty, the program almost never makes any guesses, and can solve the puzzle directly. As can be seen in the example, however, there remain situations in which the program is unable to proceed without making several guesses.

Once the program has determined the solution, it will report both the initial board and the final board as follows, along with the execution time and guesses made:

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

Execution Time: 0.0277784 seconds
Guesses Made: 11
```

This example is one of the most challenging solveable boards available, since it begins with only seventeen hints. This is the least number of hints a sudoku board may contain and still have a unique solution.
