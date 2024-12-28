#include "s21_grep.h"





void display_text(const char* filename, int file_count, char** patterns, int pattern_count, 
                int e_flag,int i_flag,int v_flag,int c_flag,int l_flag,int n_flag,
                int h_flag,int s_flag,int f_flag,int o_flag){
    // Mark unused parameters to suppress warnings TEMPORARILY
    (void)e_flag;
    (void)f_flag;
    (void)o_flag;
    
    FILE *file;
    if (!strcmp(filename, "stdin")) {
        file = stdin; // stdin
    } else {
        file = fopen(filename, "r");
        if (file == NULL) {
            if (!s_flag) printf("grep: %s: No such file or directory\n", filename);
            return;
        }
    }

    char buffer[BUFSIZ];
    char lower_b[BUFSIZ]; // case-insensitive
    int matching_line_count = 0;
    int actual_line_count = 1;
    int at_line_start = 1;

    while (fgets(buffer, BUFSIZ, file) != NULL) {
        int match_found = 0;
        at_line_start = 1;
        // if -i case insensitive
        if (i_flag) {
            to_lowercase(lower_b, buffer);
        } else {
            strcpy(lower_b, buffer);
        }

        char *current = lower_b; // for matching
        char *original = buffer; // for printing

        for (int i = 0; i < pattern_count; i++) {
            char lower_p[BUFSIZ];
                if (i_flag) {
                    to_lowercase(lower_p, patterns[i]);
                } else {
                    strcpy(lower_p, patterns[i]);
                }

                char *match;
                while ((match = strstr(current, lower_p)) != NULL) {
                    match_found = 1;
                    if (v_flag || c_flag  || l_flag) {
                        break;
                    }

                    int match_len = strlen(lower_p);
                    int match_start = match - current;
                    

                    // print before the match
                    if (n_flag && at_line_start) {
                        printf("\033[32m%d\033[36m:\033[0m",actual_line_count);
                        at_line_start = 0;
                    }
                    if (!h_flag && at_line_start && file_count > 1) {
                        printf("\033[35m%s\033[36m:\033[0m", filename);
                        at_line_start = 0;
                    }

                    if (o_flag){
                        printf("\033[1;31m%.*s\033[0m\n", match_len, &original[match_start]);
                        current = match + match_len;
                        original += match_start + match_len;
                        continue;
                    }

                    printf("%.*s", match_start, original);

                    // highlight the match
                    printf("\033[1;31m%.*s\033[0m", match_len, &original[match_start]);

                    // move pointers forward past the match
                    current = match + match_len;
                    original += match_start + match_len;

                }
            if (c_flag && match_found) matching_line_count++;
                
        }
        if (l_flag && match_found) { // l flag print filename with purple color
            printf("\033[35m%s\033[0m\n", filename);
            if (fclose(file) != 0){
                if (!s_flag) printf("Error closing file\n");
            }
            return;
        }
        // print the remainder of the line only once after all matches
        if (match_found && !v_flag && !c_flag && !o_flag) {
            printf("%s", original);
        } else if (v_flag && !match_found && !c_flag) { // Print the line if no matches were found
            printf("%s", original);
        }
        actual_line_count++;
    }
    if (c_flag) printf("%d\n", matching_line_count);
    if (fclose(file) != 0) {
        if (!s_flag) printf("Error closing file\n");
    }
}