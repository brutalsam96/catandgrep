#include "s21_cat.h"
#include <stdio.h>

void display_text(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("cat: %s: No such file or directory\n", filename);
        return;
    }
    int ch;
    while ((ch = fgetc(file)) != EOF){
        putchar(ch);
    }
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

void display_text_b(const char* filename, int* line_count){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("cat: %s: No such file or directory\n", filename);
        return;
    }
    char buffer[BUFSIZ];
    while((fgets(buffer, BUFSIZ, file)) != NULL){
        if (buffer[0] == '\n'){
            printf("\n");
        }
        else{
            printf("%6d\t%s",*line_count, buffer);
            (*line_count)++;
        }
    }
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

void display_text_e(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("cat: %s: No such file or directory\n", filename);
        return;
    }
    int ch, next_ch;
    while ((ch = fgetc(file)) != EOF){
        if (ch == '\r'){
            next_ch = fgetc(file);
            if (next_ch == '\n'){
                printf("^M$\n");
            } else {
                if (next_ch == EOF){
                    ungetc(next_ch, file);
                }
            }
        } else if (ch == '\n'){
            printf("$\n");
        } else {
            putchar(ch);
        }
    }
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

void display_text_n(const char* filename, int* line_count){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("cat: %s: No such file or directory\n", filename);
        return;
    }
    char buffer[BUFSIZ];
    while((fgets(buffer, BUFSIZ, file)) != NULL){
    printf("%6d\t%s",*line_count, buffer);
    (*line_count)++;
    }
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
    
}

void display_text_s(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("cat: %s: No such file or directory\n", filename);
        return;
    }
    char buffer[BUFSIZ];
    char buffer_next[BUFSIZ];
    while((fgets(buffer, BUFSIZ, file)) != NULL){
        if (fgets(buffer_next, BUFSIZ, file) != NULL && buffer[0] == '\n' && buffer_next[0] == '\n'){
            continue;
        } else{
            printf("%s", buffer);
        }
    }
    if (fclose(file) != 0) {
        perror("Error closing file");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage %s filename\n", argv[0]);
        return 1;
    }
    int b_flag = 0;
    int e_flag = 0;
    int n_flag = 0;
    int s_flag = 0;
    int start = 1;
    for (int i = start; i < argc; i++){
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'b'){
                b_flag = 1;
            }
            else if (argv[i][1] == 'e')
            {
                e_flag = 1;
            }
            else if (argv[i][1] == 'n')
            {
                n_flag = 1;
            }
            else if (argv[i][1] == 's')
            {
                s_flag = 1;
            }
        }
        else{
            start = i;
            break;
        }
    }
    int line_count = 1, (*line_count_ptr) = &line_count;
    for (int i = start; i < argc; i++){
        if      (b_flag)
            display_text_b(argv[i], line_count_ptr);
        else if (e_flag)
            display_text_e(argv[i]);
        else if (n_flag)
            display_text_n(argv[i], line_count_ptr);
        else if (s_flag)
            display_text_s(argv[i]);
        else
            display_text(argv[i]);
    }
    return 0;
}
