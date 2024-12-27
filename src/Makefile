G = gcc
GCCFLAGS = -Wall -Wextra -Werror -std=c11

s21_cat: cat/s21_cat.c
	$(G) $(GCCFLAGS) -o cat/s21_cat cat/s21_cat.c

s21_grep: grep/s21_grep.c grep/s21_grep.h  grep/string_utils.c grep/match_pattern.c
	$(G) $(GCCFLAGS) -o grep/s21_grep grep/s21_grep.c grep/s21_grep.h grep/string_utils.c grep/match_pattern.c
	

clean:
	rm -f cat/s21_cat grep/s21_grep