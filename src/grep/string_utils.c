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