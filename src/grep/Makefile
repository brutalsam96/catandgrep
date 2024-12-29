G = gcc
GCCFLAGS = -Wall -Wextra -Werror -std=c11

s21_grep: s21_grep.c s21_grep.h ../common/string_utils.c match_pattern.c
	$(G) $(GCCFLAGS) -o s21_grep s21_grep.c s21_grep.h ../common/string_utils.c match_pattern.c
	

clean:
	rm -f cat/s21_cat s21_grep