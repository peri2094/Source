#ifndef SUDOKU_H_
#define SUDOKU_H_

#include "hw_config.h"

void solve(int row, int col, int(*ary)[9], int(*flg)[9]); 
int check(int row, int col, int num, int(*ary)[9]); 
void back_track(int row, int col, int(*ary)[9], int(*flg)[9]);
void print_sudoku(int(*ary)[9]);
void writing_sudoku(int(*ary)[9], int(*flg)[9], int size);
void hint(int(*ary)[9], int(*flg)[9], int size);
int flag_cnt(int(*flg)[9]);

#endif