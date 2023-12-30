To Operate SudokuAI.cpp: 

insert file with the first line being the wanted version, and the following lines be the sudoku board
EX: 
B
0 0 0 1 6 0 9 2 5
0 0 7 0 0 0 1 8 0
0 0 0 0 8 0 0 0 6
7 8 0 2 0 0 0 0 0
0 2 5 0 1 0 6 3 0
0 0 0 0 0 8 0 1 7
1 0 0 0 7 0 0 0 0
0 6 2 0 0 0 7 0 0
8 7 4 0 2 3 0 0 0

Version A will utilize backtracking search with no forward checking, random variable order, with random value order.

Version B will utilize standard backtracking search + forward checking, but no random variable order, with random value order

Version C will utilize standard backtracking search + forward checking + MRV, MCV, and LCV Heuristic to sort the order of nodes to asses. 
