#include "s21_grep.h"

void to_lowercase(char* d, const char* s){
    while (*s) {
        *d = tolower(*s);
        d++;
        s++;
    }
    *d = '\0';
}