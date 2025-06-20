# Malloc Lab

## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [Chapter](#chapter)

- [Prerequisite](#prerequisite)
    - [Trace file](#trace-file)
    - [`memlib.c`](#memlibc)

- [Solution](#solution)
- [TO DO](#to-do)

## Introduction
- Students implement their own versions of `malloc`, `free`, and `realloc`
- This lab gives students a clear understanding of data layout and organization, and requires them to evaluate different trade-offs between space and time efficiency

- One of our favorite labs

- When students finish this one, they really understand pointers!

## Duration
- 2025/6/20 - Due: 2025/7/10

## Chapter
- Chapter 9: Virtual Memory

## Prerequisite
### Trace file
- Download trace file separately

### `memlib.c`
- Static variable comments correction, add `plus 1`

    ```c
    static char *mem_brk;        /* points to last byte of heap plus 1 */
    static char *mem_max_addr;   /* largest legal heap address plus 1 */ 
    ```

## Solution

## TO DO:
- Compile warning?

    ```
    /usr/bin/ld: memlib.o: warning: relocation in read-only section `.text'
    /usr/bin/ld: warning: creating DT_TEXTREL in a PIE
    ```