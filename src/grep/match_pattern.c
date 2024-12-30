#include "s21_grep.h"

void display_text(const char *filename, int file_count, char **patterns,
                  int pattern_count, const grep_flags *flags) {
  FILE *file;
  if (!strcmp(filename, "stdin")) {
    file = stdin;  // stdin
  } else {
    file = fopen(filename, "r");
    if (file == NULL) {
      if (!flags->s_flag)
        printf("grep: %s: No such file or directory\n", filename);
      return;
    }
  }
  regex_t *regex;
  char buffer[BUFSIZ];
  char combined_pattern[BUFSIZ];
  memset(combined_pattern, 0, sizeof(combined_pattern));
  for (int i = 0; i < pattern_count; i++) {
    if (i > 0) strcat(combined_pattern, "|");
    strcat(combined_pattern, patterns[i]);
  }
  int matching_line_count = 0;
  int actual_line_count = 1;

  regex = malloc(sizeof(regex_t));
  if (regex == NULL) {
    if (!flags->s_flag) printf("Memory allocation failed\n");
    return;
  }
  int reg_flags = REG_EXTENDED;
  reg_flags |= REG_NEWLINE;
  if (flags->i_flag) reg_flags |= REG_ICASE;

  if (regcomp(regex, combined_pattern, reg_flags) != 0) {
    if (!flags->s_flag)
      printf("Invalid combined regular expression: %s\n", combined_pattern);
    free(regex);
    return;
  }
  while (fgets(buffer, BUFSIZ, file) != NULL) {
    check_newline(buffer, BUFSIZ);
    if (flags->empty_p) {
      if (!flags->h_flag && file_count > 1)
        printf("\033[35m%s\033[36m:\033[0m", filename);
      printf("%s", buffer);
      continue;
    }
    int at_line_start = 1;
    process_line(buffer, regex, flags, &matching_line_count, actual_line_count,
                 at_line_start, file_count, filename);
    actual_line_count++;
  }
  if (flags->c_flag) {
    if (!flags->h_flag && file_count > 1)
      printf("\033[35m%s\033[36m:\033[0m", filename);
    printf("%d\n", matching_line_count);
  }
  if (flags->l_flag && matching_line_count && file_count > 1)
    printf("\033[35m%s\033[0m\n", filename);
  regfree(regex);
  free(regex);
  if (file != stdin && fclose(file) != 0) {
    if (!flags->s_flag) printf("Error closing file\n");
  }
}