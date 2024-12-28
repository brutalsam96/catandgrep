#include "s21_grep.h"

void to_lowercase(char* d, const char* s){
    while (*s) {
        *d = tolower(*s);
        d++;
        s++;
    }
    *d = '\0';
}

char* strduplicate(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char* copy = malloc(len);
    if (!copy) return NULL;
    memcpy(copy, str, len);
    return copy;
}

void process_line(const char* line, regex_t *regex) {
    regmatch_t pmatch[1]; // To store match offsets
    const char *buffer = line;
    const char *cursor = buffer;
    int matched = 0;

    while (regexec(regex, cursor, 1, pmatch, 0) == 0) {
        // Print the text before the match
        printf("%.*s", (int)pmatch[0].rm_so, cursor);

        // Highlight and print the matched text
        printf("\033[1;31m%.*s\033[0m", 
               (int)(pmatch[0].rm_eo - pmatch[0].rm_so), 
               cursor + pmatch[0].rm_so);

        // Move the cursor past the matched text
        cursor += pmatch[0].rm_eo;
        matched = 1;
    }

    // Print the remaining text after the last match
    if(matched) printf("%s", cursor);
}