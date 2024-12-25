#include "s21_grep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void display_text(const char* filename, char** patterns, int pattern_count, int e_flag, int i_flag, int v_flag, int c_flag, int l_flag, int n_flag, int h_flag, int s_flag, int f_flag, int o_flag) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("grep: %s: No such file or directory\n", filename);
        return;
    }

    char buffer[BUFSIZ];
    while (fgets(buffer, BUFSIZ, file) != NULL) {
        int match_found = 0;

        if (e_flag) {
            for (int i = 0; i < pattern_count; i++) {
                char *current = buffer;
                char *match;
                while ((match = strstr(current, patterns[i])) != NULL) {
                    match_found = 1;
                    printf("%.*s", (int)(match - current), current); 
                    printf("\033[1;31m%s\033[0m", patterns[i]);      
                    current = match + strlen(patterns[i]);           
                }
                if (match_found) {
                    printf("%s", current);
                    break;
                }
            }
        } else {
            if (patterns == NULL || patterns[0] == NULL) {
                printf("No pattern provided\n");
                fclose(file);
                return;
            }
            char *current = buffer;
            char *match;
            while ((match = strstr(current, patterns[0])) != NULL) {
                match_found = 1;
                printf("%.*s", (int)(match - current), current); 
                printf("\033[1;31m%s\033[0m", patterns[0]); 
                current = match + strlen(patterns[0]);
            }
            if (match_found) {
                printf("%s", current);
            }
        }
        if (!match_found){
            continue;
        }
    }

    if (fclose(file) != 0) {
        printf("Error closing file\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
        return 1;
    }

    int e_flag = 0, i_flag = 0, v_flag = 0, c_flag = 0, l_flag = 0;
    int n_flag = 0, h_flag = 0, s_flag = 0, f_flag = 0, o_flag = 0;

    char** patterns = malloc(argc * sizeof(char*));
    int pattern_count = 0;

    char** files = malloc(argc * sizeof(char*));
    int file_count = 0;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {  // Options
            if (argv[i][1] == 'e' && i + 1 < argc) {
                e_flag = 1;
                patterns[pattern_count++] = argv[++i];
            } else if (argv[i][1] == 'i') {
                i_flag = 1;
            } else if (argv[i][1] == 'v') {
                v_flag = 1;
            } else if (argv[i][1] == 'c') {
                c_flag = 1;
            } else if (argv[i][1] == 'l') {
                l_flag = 1;
            } else if (argv[i][1] == 'n') {
                n_flag = 1;
            } else if (argv[i][1] == 'h') {
                h_flag = 1;
            } else if (argv[i][1] == 's') {
                s_flag = 1;
            } else if (argv[i][1] == 'f') {
                f_flag = 1;
            } else if (argv[i][1] == 'o') {
                o_flag = 1;
            } else {
                printf("Error: Invalid option %s\n", argv[i]);
                free(patterns);
                free(files);
                return 1;
            }
        } else {
            if (!e_flag && pattern_count == 0) {
                // First non-option argument in default mode is treated as a pattern
                patterns[pattern_count++] = argv[i];
            } else {
                // Remaining arguments are treated as file names
                files[file_count++] = argv[i];
            }
        }
    }

    if (pattern_count == 0) {
        printf("Error: At least one pattern must be specified with -e.\n");
        free(patterns);
        free(files);
        return 1;
    }

    if (file_count == 0) {
        printf("Error: No file specified.\n");
        free(patterns);
        free(files);
        return 1;
    }

    // Process files
    for (int i = 0; i < file_count; i++) {
        display_text(files[i], patterns, pattern_count, e_flag, i_flag, v_flag, c_flag, l_flag, n_flag, h_flag, s_flag, f_flag, o_flag);
    }

    free(patterns);
    free(files);
    return 0;
}
