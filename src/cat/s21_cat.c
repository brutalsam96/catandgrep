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
    fclose(file);
}

void display_text_b(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("cat: %s: No such file or directory\n", filename);
        return;
    }
    char buffer[BUFSIZ];
    int line_count = 0;
    while((fgets(buffer, BUFSIZ, file)) != NULL){
        if (buffer[0] == '\n'){
            printf("\n");
        }
        else{
            printf("%5d\t%s",line_count, buffer);
            line_count++;
        }
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage %s filename\n", argv[0]);
        return 1;
    }
    int b_flag = 0;
    int start = 1;
    for (int i = start; i < argc; i++){
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'b'){
                b_flag = 1;
            }
        }
        else{
            start = i;
            break;
        }
    }

    for (int i = start; i < argc; i++){
        if (b_flag)
            display_text_b(argv[i]);
        else
        display_text(argv[i]);
    }
    return 0;
}
