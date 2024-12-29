#include "s21_grep.h"


char* strduplicate(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char* copy = malloc(len);
    if (!copy) return NULL;
    memcpy(copy, str, len);
    return copy;
}

void check_newline(char *str, size_t max_size) {
    size_t len = strlen(str);
    // Ensure there's enough space in the buffer
    if (len > 0 && str[len - 1] != '\n' && len + 1 < max_size) {
        str[len] = '\n';
        str[len + 1] = '\0';
    }
}
void process_line(const char* line, regex_t *regex, const grep_flags *flags, int* match_count, int actual_line_count, int at_line_start, int file_count, const char* filename) {
    regmatch_t pmatch[1]; // To store match offsets
    const char *buffer = line;
    const char *cursor = buffer;
    int matched = 0;

    while (regexec(regex, cursor, 1, pmatch, 0) == 0) {
        if (!flags->v_flag && !flags->c_flag && !flags->l_flag){

        if (flags->n_flag && at_line_start) {
            printf("\033[32m%d\033[36m:\033[0m",actual_line_count);
            at_line_start = 0;
        }
        if (!flags->h_flag && !flags->o_flag &&  at_line_start && file_count > 1) {
            printf("\033[35m%s\033[36m:\033[0m", filename);
            at_line_start = 0;
        }
        if (flags->o_flag){
            printf("\033[35m%s\033[36m:\033[0m", filename);
            printf("\033[1;31m%.*s\033[0m\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so), cursor + pmatch[0].rm_so);
            cursor += pmatch[0].rm_eo;
            matched = 1;
            continue;
        }
        // Print the text before the match
        printf("%.*s", (int)pmatch[0].rm_so, cursor);

        // Highlight and print the matched text
        
        printf("\033[1;31m%.*s\033[0m", 
               (int)(pmatch[0].rm_eo - pmatch[0].rm_so), 
               cursor + pmatch[0].rm_so);
        }
        // Move the cursor past the matched text
        cursor += pmatch[0].rm_eo;
        matched = 1;
        
    }
    (*match_count)++;
    if (!flags->c_flag){
        if (!matched && flags->v_flag) printf("%s", line);
        if(matched && !flags->v_flag && !flags->l_flag && !flags->o_flag) printf("%s", cursor); //something might be wrong here
    }
}

