#ifndef S21_GREP_H
#define S21_GREP_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void to_lowercase(char* d, const char* s);
char* strduplicate(const char* str);
void display_text(const char* filename, int file_count, char** patterns, int pattern_count, 
                int e_flag,int i_flag,int v_flag,int c_flag,int l_flag,int n_flag,
                int h_flag,int s_flag,int f_flag,int o_flag);

#endif  // S21_GREP_H


