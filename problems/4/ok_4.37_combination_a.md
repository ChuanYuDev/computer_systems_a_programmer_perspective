# Practice Problem 4.37 (Solution page 492)
Write a Y86-64 assembly-language program that causes combination A to arise and determines whether the control logic handles it correctly.

## Solution:
<!-- ```
    call proc

proc:
    xorq %rax, %rax
    jne target
    irmovq $1, %rax

target:
    ret
    irmovq $2, %rax
``` -->

- ~~The control logic will handle this like a mispredicted branch, but with a stall in the fetch stage~~
- ~~The next cycle, the PC selection logci will choose `irmovq $1, %rax` to be in the fetch stage~~
- ~~The control logic handles it correctly~~
- The problem is write a program to determine whether the control logic handles it correctly
    - Test whether the `ret` instruction is executed, therefore we need to push `rtnpt` (return pointer) into stack

```
1   irmovq stack %rsp       # Initialize stack pointer
2   irmovq rtnpt, %rax
3   pushq %rax              # Set up return pointer
4   xorq %rax, %rax
5   jne target
6   irmovq $1, %rax
7   halt

8   target:
9       ret
10      irmovq $2, %rbx
11      halt

12  rtnpt:
13      irmovq $3, %rdx     # Should not execute this
14      halt

15  .pos 0x40
16  stack:                  # stack: stack pointer
```
- If `ret` (line 9) is executed, then jump to `rtnpt` (`rtnpt` is at the top of the stack due to line 1, 2, 3), then `%rdx` will be set to 3 
- If the program executes correctly, only line 6 will be executed and then `halt`