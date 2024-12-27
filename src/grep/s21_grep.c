#include "s21_grep.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
        return 1;
    }

    int e_flag = 0, i_flag = 0, v_flag = 0, c_flag = 0, l_flag = 0;
    int n_flag = 0, h_flag = 0, s_flag = 0, f_flag = 0, o_flag = 0;

    char** patterns = malloc(argc * sizeof(char*));
    if (!patterns) {
        printf("Error: Memory allocation failed for patterns\n");
        return 1;
    }
    int pattern_count = 0;

    char** files = malloc(argc * sizeof(char*));
    if (!files) {
        printf("Error: Memory allocation failed for patterns\n");
        free(patterns);
        return 1;
    }

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
                FILE *file_pattern;
                f_flag = 1;
                if (i + 1 < argc) {
                    file_pattern = fopen(argv[++i], "r");
                } else {
                    if (!s_flag) printf("grep: -f option requires a file name\n");
                    free(patterns);
                    free(files);
                    return 1;
                }

                if (file_pattern == NULL){
                    if (!s_flag) printf("grep: %s: No such file or directory\n", argv[i]);
                    free(patterns);
                    free(files);
                    return 1;
                }
                char buffer[BUFSIZ];
                while (fgets(buffer, BUFSIZ, file_pattern)){
                    buffer[strcspn(buffer, "\n")] = '\0';
                    char *new_pattern = strduplicate(buffer);
                    if (!new_pattern) {
                        if (!s_flag) printf("Error: Memory allocation failed for patterns from file\n");
                        fclose(file_pattern);
                        free(patterns);
                        free(files);
                        return 1;
                    }
                    patterns[pattern_count++] = new_pattern;
                }
                fclose(file_pattern);
                
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
    
    if (!isatty(STDIN_FILENO)){
        display_text("stdin", file_count, patterns, 1, e_flag, i_flag, v_flag, c_flag, l_flag, n_flag, h_flag, s_flag, f_flag, o_flag);
        free(patterns);
        free(files);
        return 0;

    }

    if (pattern_count == 0) {
        if (!s_flag) printf("Error: At least one pattern must be specified with -e.\n");
        free(patterns);
        free(files);
        return 1;
    }

    if (file_count == 0) {
        if (!s_flag) printf("Error: No file specified.\n");
        free(patterns);
        free(files);
        return 1;
    }

    // Process files
    for (int i = 0; i < file_count; i++) {
            display_text(files[i], file_count, patterns, pattern_count, e_flag, i_flag, v_flag, c_flag, l_flag, n_flag, h_flag, s_flag, f_flag, o_flag);
        }
        // if (i < file_count - 1 && !l_flag) {
        //     printf("\n");
        // }

    free(patterns);
    free(files);
    return 0;
}
