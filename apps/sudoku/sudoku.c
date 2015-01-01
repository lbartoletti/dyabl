#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/*
 * tableau[9][9] = {
 *  {8,0,0,0,4,0,1,0,2},
 *  {0,0,2,0,0,0,0,3,0},
 *  {0,5,0,9,0,0,0,0,4},
 *  {0,0,1,8,3,0,0,0,0},
 *  {9,0,0,2,5,4,0,0,6},
 *  {0,0,0,0,9,1,8,0,0},
 *  {7,0,0,0,0,5,0,6,0},
 *  {0,1,0,0,0,0,4,0,0},
 *  {6,0,4,0,8,0,0,0,5}
 * };
 */

static bool isNotOnLine(int val, int arr[9][9], int line) {
  int col;
  for (col = 0; col < 9; col++)
    if (arr[line][col] == val)
      return false;

  return true;
}

static bool isNotOnCol(int val, int arr[9][9], int col) {
  int line;
  for (line = 0; line < 9; line++)
    if (arr[line][col] == val)
      return false;

  return true;
}

static bool isNotOnSubRegion(int val, int arr[9][9], int line, int col) {
  int s_col = col - (col % 3);
  int s_line = line - (line % 3);

  for (int i = s_line; i < s_line + 3; i++)
    for (int j = s_col; j < s_col + 3; j++)
      if (arr[i][j] == val)
        return false;

  return true;
}

static bool resolveSudoku(int arr[9][9], int cell) {
  if (cell == 9 * 9)
    return true;

  int i = cell / 9, j = cell % 9;

  if (arr[i][j] != 0)
    return resolveSudoku(arr, cell + 1);

  for (int p = 1; p <= 9; p++) {
    if (isNotOnLine(p, arr, i) && isNotOnCol(p, arr, j) &&
        isNotOnSubRegion(p, arr, i, j)) {
      arr[i][j] = p;
      if (resolveSudoku(arr, cell + 1))
        return true;
    }
  }

  arr[i][j] = 0;
  return false;
}

static void printTab(int arr[9][9]) {
  for (int i = 0; i < 9; i++) {
    if (i % 3)
      puts("+         +         +         +");
    else
      puts("+---------+---------+---------+");
    for (int j = 0; j < 9; j++) {
      printf("%s%d ", (j % 3) ? " " : "| ", arr[i][j]);
    }
    printf("|\n");
  }
  puts("+---------+---------+---------+");
}

int main(void) {
  int tableau[9][9] = {{8, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 3, 6, 0, 0, 0, 0, 0},
                       {0, 7, 0, 0, 9, 0, 2, 0, 0}, {0, 5, 0, 0, 0, 7, 0, 0, 0},
                       {0, 0, 0, 0, 4, 5, 7, 0, 0}, {0, 0, 0, 1, 0, 0, 0, 3, 0},
                       {0, 0, 1, 0, 0, 0, 0, 6, 8}, {0, 0, 8, 5, 0, 0, 0, 1, 0},
                       {0, 9, 0, 0, 0, 0, 4, 0, 0}};
  printTab(tableau);

  resolveSudoku(tableau, 0);
  printTab(tableau);

  return 0;
}
