#include "sudoku.h"
#include "motor.h"
#include "timer.h"

extern int locx, locy;
unsigned int counter;

//--------Number Input to empty location--------//
void solve(int row, int col, int(*ary)[9], int(*flg)[9])
{  
  int num;
  if (ary[row][col] != 0) { flg[row][col] = 1; return; }

  for (num = 1; num <= 9; num++)
  {
     if (check(row, col, num, ary) == 1)
     {
        ary[row][col] = num;
	return;
     }
  }
  back_track(row, col, ary, flg);
}

//--------Number checking 1 to 9--------//
int check(int row, int col, int num, int(*ary)[9])
{
  int i, j;
  int a = (row / 3) * 3;
  int b = (col / 3) * 3;

  for (i = 0; i < 9; i++) { if (ary[row][i] == num ) { return 0; } }
  for (i = 0; i < 9; i++) { if (ary[i][col] == num) { return 0; } }

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++) { if (ary[a + i][b + j] == num) { return 0; } }
  }
   return 1;
}

//--------Backtraking to revise Number--------//
void back_track(int row, int col, int(*ary)[9], int(*flg)[9])
{
  int temp = 0, i;

  while (1)
  {
    col--;
    if (col == -1) {row--; col = 8;}
    if (flg[row][col] != 1) { locx = row; locy = col;  break; }
  }
  temp = ary[row][col] + 1;

  if (temp > 9)
  {
    ary[row][col] = 0;
    back_track(row, col, ary, flg);
    return;
  }

  for (i = temp; i <= 9; i++)
  {
    if (check(row, col, i, ary) == 1)
    {
     ary[row][col] = i;
     break;
    }
    if (i == 9)
    {
     ary[row][col] = 0;
     back_track(row, col, ary, flg);
    }
  }
  return;
}

void print_sudoku(int(*ary)[9])
{
  int cnt = 0, i = 0;

  printf("\n###### Result ######\n\n");

  while(1)
  {
    printf("%d  ", ary[i][cnt++]);

    if (cnt == 9)
    {
      printf("\n");
      i++;
      if (i == 9) { break; }
      cnt = 0;
    }
  }
}

//--------Writing Number in empty cell--------//
void writing_sudoku(int(*ary)[9], int(*flg)[9], int size)
{
  int cnt = 0, i = 0;
  
  Axis_Control(corr_x, corr_y); 
  
  while(1)
  {
    if(flg[i][cnt] == 0) {Print_Num(ary[i][cnt], size);}
    cnt++;
    
    if (cnt == 9)
    {
      i++;
      cnt = 0;
      if (i == 9) {i = 0; Axis_Control(corr_x, corr_y); break; }
    }
    Axis_Control(corr_x + (size * cnt), corr_y - (size * i));
  }
}

//--------Count Flag's Number--------//
int flag_cnt(int(*flg)[9])
{
  int i, j;
  int cnt = 0;
  
  for(i = 0; i< 9; i++)
  {
    for(j = 0; j < 9; j++)
    {
      if(flg[i][j] == 0) {cnt++;}
    }
  }
  return cnt;
}

//--------Writing number random position--------//
void hint(int(*ary)[9], int(*flg)[9], int size)
{
  static int cnt = 0;
  int i, j;
  
  if(cnt == 0) { cnt = flag_cnt(flg); }
  
  while(cnt != 0)
  { 
    Counter_OnOff();
    srand(counter);
    i = rand() % 9;
    j = rand() % 9;
    
    if(flg[i][j] == 0)
    {
      Axis_Control(corr_x + (size * j), corr_y - (size * i));
      Print_Num(ary[i][j], size);
      flg[i][j] = 1;
      cnt--;
      if(cnt == 0) { printf("All empty cells solved!\n"); exit(0); }
      Counter_OnOff();
      break;
    }
  }
}