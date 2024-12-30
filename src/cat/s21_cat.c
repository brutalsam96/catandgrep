#include "s21_cat.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display_text(const char* filename, int b_flag, int e_flag, int n_flag,
                  int s_flag, int t_flag) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", filename);
    return;
  }

  int ch, prev_ch = '\n';
  int line_count = 1;

  while ((ch = fgetc(file)) != EOF) {
    // -s (squeeze blank lines)
    if (s_flag && ch == '\n' && prev_ch == '\n') {
      continue;
    }

    // -n or -b (line numbering)
    if ((n_flag || b_flag) && prev_ch == '\n') {
      if (!b_flag || ch != '\n') {
        printf("%6d\t", line_count++);
      }
    }

    // -e (end-of-line markers)
    if (e_flag && ch == '\n') {
      putchar('$');
    }

    // -t (tab markers)
    if (t_flag && ch == '\t') {
      printf("^I");
    } else if(t_flag && ch == '\r') {
      printf("^M");
    }
    else {
      putchar(ch);
    }

    prev_ch = ch;
  }

  if (fclose(file) != 0) {
    perror("Error closing file");
  }
}

void parse_flags_and_files(int argc, char* argv[], int* b_flag, int* e_flag, int* n_flag, int* s_flag, int* t_flag, char* filenames[], int* file_count) {
  *file_count = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        switch (argv[i][j]) {
          case 'b':
            *b_flag = 1;
            *n_flag = 0;  // -b overrides -n
            break;
          case 'e':
            *e_flag = 1;
            break;
          case 'n':
            if (!*b_flag) *n_flag = 1;  // -b has higher priority
            break;
          case 's':
            *s_flag = 1;
            break;
          case 't':
            *t_flag = 1;
            break;
          default:
            fprintf(stderr, "Unknown option: -%c\n", argv[i][j]);
            exit(1);
        }
      }
    } else {
      filenames[*file_count] = argv[i];
      (*file_count)++;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s [-bens] filename...\n", argv[0]);
    return 1;
  }

  int b_flag = 0, e_flag = 0, n_flag = 0, s_flag = 0, t_flag = 0;
  char* filenames[argc - 1];
  int file_count = 0;

  // Parse flags and filenames
  parse_flags_and_files(argc, argv, &b_flag, &e_flag, &n_flag, &s_flag, &t_flag, filenames, &file_count);

  if (file_count == 0) {
    fprintf(stderr, "No files provided.\n");
    return 1;
  }

  // Process files
  for (int i = 0; i < file_count; i++) {
    display_text(filenames[i], b_flag, e_flag, n_flag, s_flag, t_flag);
  }

  return 0;
}
