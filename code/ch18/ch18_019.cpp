Initial board:
  5 3 0 | 0 7 0 | 0 0 0
  6 0 0 | 1 9 5 | 0 0 0
  0 9 8 | 0 0 0 | 0 6 0
  ------+-------+------
  8 0 0 | 0 6 0 | 0 0 3
  4 0 0 | 8 0 3 | 0 0 1
  7 0 0 | 0 2 0 | 0 0 6
  ------+-------+------
  0 6 0 | 0 0 0 | 2 8 0
  0 0 0 | 4 1 9 | 0 0 5
  0 0 0 | 0 8 0 | 0 7 9

Step 1:  Row 0, Col 2 (empty).
         Try 1: row 0 has no 1, col 2 has 8,6 -> no 1,
                box(0,0) has 5,3,6,9,8 -> no 1
                -> place 1. board[0][2] = 1.
Step 2:  Row 0, Col 3 (empty).
         Try 1: row 0 now has 1 -> conflict.
         Try 2: row 0 has no 2, col 3 has 1,8,4 -> no 2,
                box(0,3) has 7,1,9,5 -> no 2
                -> place 2. board[0][3] = 2.
         ... continue placing digits in row 0 ...
Step 3:  Row 0, Col 5 (empty).
         Try all digits 1-9: all conflict with row, col, or box.
         -> BACKTRACK: reset Col 3 to 0, try next digit.
         Try 3 for Col 3: row 0 has no 3, col 3 has 1,8,4 -> no 3,
                box(0,3) has 7,1,9,5 -> no 3.
                -> place 3. board[0][3] = 3.
         ... continue ...
         
This process continues, exploring and backtracking until the
board is filled. Hard puzzles (17 clues) may require millions
of backtracks.
