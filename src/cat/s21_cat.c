#include "s21_cat.h"

#include <stdio.h>

void display_text(const char* filename, int b_flag, int e_flag, int n_flag,
                  int s_flag, int t_flag) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    return;
  }

  int ch, prev_ch = EOF, line_count = 1, blank_line = 0;
  while ((ch = fgetc(file)) != EOF) {
    // -s (squeeze blank lines)
    if (s_flag && ch == '\n' && prev_ch == '\n') {
      if (!blank_line) {
        putchar(ch);
        blank_line = 1;
      }
      continue;
    }
    blank_line = 0;

    // -n or -b (line numbering)
    if ((n_flag || b_flag) && (prev_ch == '\n' || prev_ch == EOF)) {
      if (!b_flag || ch != '\n') {
        printf("%6d\t", line_count++);
      }
    }

    // -e (end-of-line markers) and -t (tab markers)
    if (e_flag && ch == '\n' && prev_ch != '\r') {
      putchar('$');
    } else if (e_flag || (t_flag && ch == '\r')) {
      putchar('^');
      putchar('M');
      continue;
    } else if (t_flag && ch == '\t') {
      printf("^I");
      continue;
    }
    putchar(ch);
    prev_ch = ch;
  }
  if (fclose(file) != 0) {
    perror("Error closing file");
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage %s filename\n", argv[0]);
    return 1;
  }
  int b_flag = 0;
  int e_flag = 0;
  int n_flag = 0;
  int s_flag = 0;
  int t_flag = 0;
  int start = 1;
  for (int i = start; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'b') {
        b_flag = 1;
      }
      if (argv[i][1] == 'e') {
        e_flag = 1;
      }
      if (argv[i][1] == 'n') {
        n_flag = 1;
      }
      if (argv[i][1] == 's') {
        s_flag = 1;
      }
      if (argv[i][1] == 't') {
        t_flag = 1;
      }
    } else {
      start = i;
      break;
    }
  }
  // int line_count = 1, (*line_count_ptr) = &line_count;
  for (int i = start; i < argc; i++) {
    display_text(argv[i], b_flag, e_flag, n_flag, s_flag, t_flag);
  }
  return 0;
}
