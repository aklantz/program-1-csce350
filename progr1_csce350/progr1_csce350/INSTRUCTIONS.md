# Program 1 (CSCE 350)

## Description

You will code Heapsort, an Anagram checker, and three binary search tree algorithms.

This is shorter than usual because you may need more time to get your environment
set up (you need Linux). Your code must run on the department Linux machines (1D43).
If you have a separate Linux installation that you want to use, ssh over to the
department machines to make sure nothing breaks, or just go visit them.

This assignment builds as **C++20**. You need g++ 10 or newer and cmake 3.16 or
newer. Check with `g++ --version` and `cmake --version`. The lab machines have
both; if your own machine does not, that is the reason to ssh over rather than
debug it locally.

[Segfault Troubleshooting Strategies](link)

## SSH Instructions

In Putty/MobaXTerm/WinSCP/Bitvise/... or just plain ssh:

SSH to `l-1d43-XY.cse.sc.edu` on **port 222**

- first character is a lowercase L
- `XY` = 01 | 02 | ... | 36
- If you're off campus, you will need to have Duo 2 Factor Authentication set up
  and use CiscoVPN.

From a non-lab Linux machine:

```
ssh -p 222 <username>@l-1d43-XY.cse.sc.edu
```

## Instructions

Download the zip file and unzip it where you would work.

You can run the scoring function that builds and runs the tests for you and prints
your score by running the following command (cd into the directory first):

```
python3 score.py
```

You should see `SCORE= 0` before you get any work done.

The first build downloads googletest, so it needs network access once. After that
it is cached in `build/_deps` and later builds work offline. If a `googletest/`
directory came with the assignment, that is used instead and nothing is downloaded.

You can run the unit tests directly without using the score function (not
necessary) with the following commands, also from the downloaded directory:

```
cd program-1-<username>
mkdir build && cd build
cmake ..
cmake --build .
./runUnitTests
```

### Reproducing a failure

The tests use random data, so a failure may not repeat on the next run. Every run
prints its seed:

```
[  SEED    ] 1788909723  (re-run with TEST_SEED=1788909723 to reproduce)
```

Re-run with that seed to get the exact same test data back:

```
TEST_SEED=1788909723 ./runUnitTests
```

Run a single test while you work on it:

```
./runUnitTests --gtest_filter=dup_val*
```

The build turns on bounds checking for `std::vector` and `std::string`. If you
index past the end of one, the program stops immediately with a message naming
the file and line instead of quietly reading garbage. That message is a bug in
your code, not in the tests.

---

## Question 1: Implement Heap Sort (algs.h)

- You should use `std::priority_queue` (already `#include`d from `<queue>`)
  - See: https://en.cppreference.com/w/cpp/container/priority_queue
- My implementation, including a closing brace, is 5 lines. DRTW
- Your solution does not have to be "in place", meaning you can use significant
  extra space (this means your heapsort is really almost-heapsort). The purpose of
  this question is to use an STL templated data structure... look for examples
  online to figure out the basics of `std::priority_queue`.
- Sort into **ascending** order, and leave the vector the same size you found it.

## Question 2: Implement an Anagram Checker (algs.h)

- Your function, given two strings, should return true if the two strings could be
  anagrams of one another and false otherwise.
- Two strings are anagrams of one another if one can rearrange the characters of
  one string to equal the other.
- You can assume all strings are composed of characters 'A'..'Z'.
- Obviously, "BBB" is not an anagram of "CAB"... Summing the characters is not correct.
- Your answer must run in O(m+n+k) time, where m and n are the lengths of the two
  strings and k is the number of characters in the character set (26, here). This means:
  - You cannot sort the strings with a typical (comparison-based) sort, and don't
    need to sort at all really.
  - You cannot read the strings more than a fixed number of times per character
    (you can do this by visiting each string character exactly once).
- A helper function is a good idea here (to convert characters to something...).

## Question 3: Implement num_nodes (tree.h)

- It returns the number of nodes in the tree.

## Question 4: Implement has_duplicate_val (tree.h)

- It returns true if the tree has duplicate values/keys in it, false otherwise.
- Hint: mentally build the tree by inserting, left to right and in order, the
  values 4 6 2 4. Notice anything? In particular, notice where the second 4 lands
  relative to the first — checking only whether a node equals its own parent is
  not enough.

## Question 5: Implement trees_identical (tree.h)

- It returns true if the two trees, passed as pointer parameters, are the same in
  structure and node values, and false otherwise.
- Either or both pointers may be null.

Test as above, one last time: all five tests should pass, for `SCORE= 50`.

---

## Committing

Zip the files and upload. Do not include your `build/` directory.

We will grade the last submission up to the deadline (the very last submission
until we won't accept it anymore, possibly with a late penalty).

See syllabus for late policy.
