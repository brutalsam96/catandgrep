#include <stdio.h>

void display_text(const char* filename, int b_flag, int e_flag, int n_flag,
                  int s_flag, int t_flag);
void display_text_b(const char* filename, int* line_count);
void display_text_e(const char* filename);
void display_text_n(const char* filename, int* line_count);
void display_text_s(const char* filename);
void display_text_t(const char* filename);