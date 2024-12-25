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

void display_text_b(const char* filename, int* line_count) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    return;
  }
  char buffer[BUFSIZ];
  while ((fgets(buffer, BUFSIZ, file)) != NULL) {
    if (buffer[0] == '\n') {
      printf("\n");
    } else {
      printf("%6d\t%s", *line_count, buffer);
      (*line_count)++;
    }
  }
  if (fclose(file) != 0) {
    perror("Error closing file");
  }
}

void display_text_e(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    return;
  }
  int ch, next_ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\r') {
      next_ch = fgetc(file);
      if (next_ch == '\n') {
        printf("^M$\n");
      } else {
        if (next_ch == EOF) {
          ungetc(next_ch, file);
        }
      }
    } else if (ch == '\n') {
      printf("$\n");
    } else {
      putchar(ch);
    }
  }
  if (fclose(file) != 0) {
    perror("Error closing file");
  }
}

void display_text_n(const char* filename, int* line_count) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    return;
  }
  char buffer[BUFSIZ];
  while ((fgets(buffer, BUFSIZ, file)) != NULL) {
    printf("%6d\t%s", *line_count, buffer);
    (*line_count)++;
  }
  if (fclose(file) != 0) {
    perror("Error closing file");
  }
}

void display_text_s(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    return;
  }
  char buffer[BUFSIZ];
  char buffer_next[BUFSIZ];
  while ((fgets(buffer, BUFSIZ, file)) != NULL) {
    if (fgets(buffer_next, BUFSIZ, file) != NULL && buffer[0] == '\n' &&
        buffer_next[0] == '\n') {
      continue;
    } else {
      printf("%s", buffer);
    }
  }
  if (fclose(file) != 0) {
    perror("Error closing file");
  }
}

void display_text_t(const char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("cat: %s: No such file or directory\n", filename);
    return;
  }
  int ch, next_ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\r') {
      next_ch = fgetc(file);
      if (next_ch == '\n') {
        printf("^M\n");
      } else {
        if (next_ch == EOF) {
          ungetc(next_ch, file);
        }
      }
    } else if (ch == '\t') {
      printf("^I");
    } else if (ch == '\n') {
      printf("\n");
    } else {
      putchar(ch);
    }
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
