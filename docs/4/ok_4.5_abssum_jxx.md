# Practice Problem 4.5 (solution page 483)
Modify the Y86-64 code for the sum function (Figure 4.6) to implement a function `absSum` that computes the sum of absolute values of an array. Use a conditional instruction within your inner loop.

## Solution:

- Figure 4.6:
    ```
    # long absSum(long *start, long count)
    # start in %rdi , count in %rsi
    1   sum:
    2       irmovq $8, %r8      # Constant 8
    3       irmovq $1, %r9      # Constant 1
    4       xorq %rax, %rax     # sum = 0
    5       andq %rsi, %rsi     # Set CC
    6       jmp test            # Goto test
    7   loop:
    8       mrmovq (%rdi), %r10 # Get *start
    9       addq %r10, %rax     # Add to sum
    10      addq %r8, %rdi      # start++
    11      subq %r9, %rsi      # count--. Set CC
    12  test:
    13      jne loop            # Stop when 0
    14      ret                 # Return
    ```

- My answer:
    ```
    # long absSum(long *start, long count)
    # start in %rdi , count in %rsi
    1   absSum:
    2       irmovq $8, %r8      # Constant 8
    3       irmovq $1, %r9      # Constant 1
    4       xorq %rax, %rax     # sum = 0
    5       andq %rsi, %rsi     # Set CC
    6       jmp test            # Goto test
    7   loop:
    8       mrmovq (%rdi), %r10 # Get *start
    9       andq %r10, %r10     # Compare %r10 with 0
    10      jl neg
    11      addq %r10, %rax     # Add to sum
    12      addq %r8, %rdi      # start++
    13      subq %r9, %rsi      # count--. Set CC
    14      jne loop            # Stop when 0
    15      ret                 # Return
    16  neg:
    17      subq %r10, %rax
    18
    19      addq %r8, %rdi      # start++
    20      subq %r9, %rsi      # count--. Set CC
    21  test:
    22      jne loop            # Stop when 0
    23      ret                 # Return
    ```

    - Lines 19-23 are the same with lines 12-15 
    - I need line 9 to compare %r10 with 0 and then do the arithmetic computation

- Modified answer
    ```
    # long absSum(long *start, long count)
    # start in %rdi , count in %rsi
    1   absSum:
    2       irmovq $8, %r8      # Constant 8
    3       irmovq $1, %r9      # Constant 1
    4       xorq %rax, %rax     # sum = 0
    5       andq %rsi, %rsi     # Set CC
    6       jmp test            # Goto test
    7   loop:
    8       mrmovq (%rdi), %r10 # Get *start
    9       xorq %r11, %r11
    10      subq %r10, %r11
    11      jle pos
    12      rrmovq %r11, %r10
    13  pos:
    14      addq %r10, %rax     # Add to sum
    15      addq %r8, %rdi      # start++
    16      subq %r9, %rsi      # count--. Set CC
    17  test:
    18      jne loop            # Stop when 0
    19      ret                 # Return
    ```
    - Line 9 sets `%r11` to 0 via `xorq` because 
        - `xorq` instruction is 2 bytes, while `irmovq` is 10 bytes
    - Line 11 uses `jle` instead of `jl` because
        - If the branch is not taken, we need to execute line 12
        - We want to reduce the number of instructions as much as possible, so we use looser conditions