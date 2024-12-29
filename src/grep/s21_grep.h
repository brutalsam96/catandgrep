#ifndef S21_GREP_H
#define S21_GREP_H

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// flags struct

typedef struct flags {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;
  int o_flag;
} grep_flags;
void check_newline(char* str, size_t max_size);
char* strduplicate(const char* str);
void process_line(const char* line, regex_t* regex, const grep_flags* flags,
                  int* match_count, int actual_line_count, int at_line_start,
                  int file_count, const char* filename);
void display_text(const char* filename, int file_count, char** patterns,
                  int pattern_count, const grep_flags* flags);

#endif  // S21_GREP_H
