#!/bin/bash

# Define the paths for the executables
CAT_EXEC="./cat/s21_cat"
GREP_EXEC="./grep/s21_grep"

# Test cases for s21_cat
cat_tests=(
  "cat/2.txt"
  "cat/2.txt -b"
  "cat/2.txt -n"
  "cat/2.txt -e -s"
  "cat/2.txt -t"
  "cat/2.txt -s"
  "cat/2.txt cat/3.txt -s"
)

# Test cases for s21_grep
grep_tests+=(
  # Basic flag tests
  "error grep/1.txt"
  "-i error grep/1.txt"
  "-v error grep/1.txt"
  "-c error grep/1.txt"

  # Special characters
  "-e '@#$%' grep/special_chars.txt"
  "-n '[];' grep/special_chars.txt"
  "-h '|' grep/special_chars.txt"
  "-l '@#$' grep/special_chars.txt"

  # Unicode examples
  "こんにちは grep/unicode.txt"
  "-i مرحبا grep/unicode.txt"
  "-v 안녕하세요 grep/unicode.txt"
  "-c 你好 grep/unicode.txt"

  # Mixed flags
  "-n -h 'This' grep/test.txt"
  "-i -v 'World' grep/test.txt"
  "-l -n -c 'test' grep/test.txt"
  "-e 'error' -n -v grep/2.txt"
  

  # Testing additional files
  "-i -c 'error' grep/2.txt"
  "-n -h 'error' grep/2.txt"
  "-i -v 'error' grep/2.txt"
  "-e '|' -v grep/special_chars.txt"
)

# Run Valgrind on s21_cat
run_valgrind() {
  local exec=$1
  local test_case=$2

  echo "Running Valgrind on $exec with arguments: $test_case"
  valgrind --leak-check=full --track-origins=yes $exec $test_case
  echo "---------------------------------------------------"
}

# Test s21_cat
# for test in "${cat_tests[@]}"; do
#   run_valgrind $CAT_EXEC "$test"
# done

# Test s21_grep
for test in "${grep_tests[@]}"; do
  run_valgrind $GREP_EXEC "$test"
done
