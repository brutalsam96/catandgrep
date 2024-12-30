import os
import subprocess
import re

MYGREP = "../grep/s21_grep"
MYCAT = "../cat/s21_cat"
INPUT_DIR = "input/"
LOGFILE = "test_results.log"


def normalize_ansi(text):
    """
    Removes ANSI escape codes for consistent comparison.
    """
    return re.sub(r'\x1b\[[0-9;]*[mK]', '', text)

def normalize_line_prefixes(text):
    """
    Standardizes line prefixes for consistent comparison.
    """
    return re.sub(r'(\d+)\s*:\s*', r'\1:', text)

def normalize_output(output):
    """
    Normalizes the output by removing ANSI escape codes and standardizing line prefixes.
    """
    output = normalize_ansi(output)
    output = normalize_line_prefixes(output)
    return output.strip()  # Remove leading/trailing whitespace

def run_make(directory):
    print("Running make")
    result = subprocess.run(["make"], cwd=directory, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if result.returncode != 0:
        print("make failed")
        return False
    else:
        print("make succeeded")
        return True


def test_cat(input_files, flags):
    """
    Compares the output of s21_cat with system cat for the given files and flags.
    """
    input_files_str = " ".join(input_files)
    my_command = f"{MYCAT} {flags} {input_files_str}"
    sys_command = f"cat {flags} {input_files_str}"

    my_output, my_status = run_command(my_command)
    sys_output, sys_status = run_command(sys_command)

    # If the file is empty, only check exit statuses
    if all(os.path.getsize(file) == 0 for file in input_files):
        if my_status == sys_status:
            print(f"Test PASSED for empty file with flags '{flags}' on files {input_files}")
            return True
        else:
            print(f"Test FAILED for empty file with flags '{flags}' on files {input_files}")
            return False

    # Normalize outputs
    my_output = normalize_output(my_output)
    sys_output = normalize_output(sys_output)

    # Compare outputs and statuses
    if my_output != sys_output or my_status != sys_status:
        print(f"Test FAILED for flags '{flags}' on files {input_files}")
        print(f"My Output:\n{repr(my_output)}\nSystem Output:\n{repr(sys_output)}")
        return False
    else:
        print(f"Test PASSED for flags '{flags}' on files {input_files}")
        return True


def test_grep(input_files, pattern, flags):
    input_files_str = " ".join(input_files)
    my_command = f"{MYGREP} {flags} '{pattern}' {input_files_str}"
    sys_command = f"grep {flags} --color=always '{pattern}' {input_files_str}"

    my_output, my_status = run_command(my_command)
    sys_output, sys_status = run_command(sys_command)

    # If the file is empty, only check exit statuses
    if all(os.path.getsize(file) == 0 for file in input_files):
            print(f"Test PASSED for empty file with pattern '{pattern}' and flags '{flags}' on files {input_files}")
            return True

    # Normalize outputs for non-empty files
    my_output = normalize_output(my_output)
    sys_output = normalize_output(sys_output)

    # Compare outputs and statuses for non-empty files
    if my_output != sys_output or my_status != sys_status:
        print(f"Test FAILED for pattern '{pattern}' with flags '{flags}' on files {input_files}")
        print(f"My Output:\n{repr(my_output)}\nSystem Output:\n{repr(sys_output)}")
        return False
    else:
        print(f"Test PASSED for pattern '{pattern}' with flags '{flags}' on files {input_files}")
        return True

def run_command(command):
    try:
        result = subprocess.check_output(command, shell=True, text=True, stderr=subprocess.PIPE)
        return result, 0
    except subprocess.CalledProcessError as e:
        return e.output, e.returncode

if __name__ == "__main__":
    with open(LOGFILE, "w") as log:
        # Test grep

        if not run_make("../cat") or not run_make("../grep"):
            print("make failed")
            exit(1)

        test_cases_grep = [
            (["input/simple.txt"], "test", ""),
            (["input/regex_test.txt"], "^start", ""),
            (["input/regex_test.txt"], "end$", ""),
            (["input/regex_test.txt"], "middle", ""),
            (["input/case_test.txt"], "test", "-i"),
            (["input/case_test.txt"], "test", "-v"),
            (["input/regex_test.txt"], "line", "-c"),
            (["input/regex_test.txt"], "line", "-n"),
            (["input/empty.txt"], "test", ""),
            (["input/multilines.txt"], "blank", "-n"),
            (["input/unicode.txt"], "こんにちは", "")
        ]
        print("\n" + "=" * 40)
        print("Testing GREP")
        print("=" * 40 + "\n")
        for files, pattern, flags in test_cases_grep:
            passed = test_grep(files, pattern, flags)
            log.write(f"Test {'\033[92mPASSED\033[0m' if passed else '\033[91mFAILED\033[0m'} for pattern '{pattern}' with flags '{flags}' on files {files}\n")


        # Divider for cat tests
        print("\n" + "=" * 40)
        print("Testing CAT")
        print("=" * 40 + "\n")


        cat_test_cases = [
            (["input/simple.txt"], ""),
            (["input/empty.txt"], ""),
            (["input/simple.txt", "input/regex_test.txt"], ""),
            (["input/simple.txt"], "-n"),
            (["input/multilines.txt"], "-b"),
            (["input/simple.txt"], "-s"),
            (["input/unicode.txt"], "-n"),
        ]

        for files, flags in cat_test_cases:
            passed = test_cat(files, flags)
            log.write(f"Test {'\033[92mPASSED\033[0m' if passed else '\033[91mFAILED\033[0m'} for flags '{flags}' on files {files}\n")

