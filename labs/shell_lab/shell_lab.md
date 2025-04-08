# Shell Lab

## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [Chapter](#chapter)
- [Solution](#solution)
    - [Trace01](#trace01)

## Introduction
- Students implement their own simple Unix shell program with job control, including the `ctrl-c` and `ctrl-z` keystrokes, `fg`, `bg`, and `jobs` commands

- This is the students' first introduction to application level concurrency, and gives them a clear idea of Unix process control, signals, and signal handling

## Duration
- 2025/3/22 - Due: 2025/4/14

## Chapter
- Chapter 8: Exceptional Control Flow

## Solution
### Trace01
- The `tsh.c` already implements the function of terminating properly on EOF

    ```c
    if (feof(stdin)) { /* End of file (ctrl-d) */
        fflush(stdout);
        exit(0);
    }
    ```

- Result

    ```
    make test01
    ./sdriver.pl -t trace01.txt -s ./tsh -a "-p"
    #
    # trace01.txt - Properly terminate on EOF.
    #
    ```

    ```
    make rtest01
    ./sdriver.pl -t trace01.txt -s ./tshref -a "-p"
    #
    # trace01.txt - Properly terminate on EOF.
    #
    ```
### Trace02
### Trace03
### Trace04
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01
### Trace01