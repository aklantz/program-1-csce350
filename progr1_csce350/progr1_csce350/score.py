import pathlib
import shutil
import os
import subprocess
import sys

unit_test_exec = './runUnitTests'

build_path = './build'


def run_named_test(test_exec, test_name, score_if_passed):
    try:
        subprocess.check_call([test_exec, '--gtest_filter=' + test_name + '*'])
        return score_if_passed
    except subprocess.CalledProcessError:
        return 0


# clean and remake dir
try:
    shutil.rmtree(build_path)
except FileNotFoundError:
    print('no ' + build_path + ' to delete')

pathlib.Path(build_path).mkdir(parents=True, exist_ok=True)


# make runUnitTests in build dir
old_cwd = os.getcwd()
os.chdir(build_path)

# The first configure downloads googletest unless a googletest/ directory
# was shipped with the assignment, so it needs network access once.
if subprocess.run(['cmake', '..']).returncode != 0:
    print('\ncmake failed. Your code did not build, so your score is 0.')
    print('Check that cmake is 3.16+ and g++ is 10+ (cmake --version, g++ --version).')
    print('SCORE= 0')
    os.chdir(old_cwd)
    sys.exit(1)

if subprocess.run(['cmake', '--build', '.']).returncode != 0:
    print('\nCompilation failed. Fix the errors above; your score is 0 until it builds.')
    print('SCORE= 0')
    os.chdir(old_cwd)
    sys.exit(1)

subprocess.run([unit_test_exec])

test_wts = []
test_wts.append(('heapsort_rand', 10))
test_wts.append(('ana_checker', 10))
test_wts.append(('bst_count', 7))
test_wts.append(('dup_val', 10))
test_wts.append(('tree_identical', 13))

score = 0
for (test, wt) in test_wts:
    score = score + run_named_test(unit_test_exec, test, wt)

print("SCORE= " + str(score))

os.chdir(old_cwd)
