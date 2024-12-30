#ifndef S21_CAT_H
#define S21_CAT_H

void display_text(const char* filename, int b_flag, int e_flag, int n_flag,
                  int s_flag, int t_flag);
void parse_flags_and_files(int argc, char* argv[], int* b_flag, int* e_flag, int* n_flag, int* s_flag, int* t_flag, char* filenames[], int* file_count);
#endif  // S21_CAT_H
