# Practice Problem 4.6 (solution page 483)
Modify the Y86-64 code for the sum function (Figure 4.6) to implement a function `absSum` that computes the sum of absolute values of an array. Use a conditional move instruction within your inner loop.

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
    9       rrmovq %rax, %rcx   # %rcx <- %rax 
    10      subq %r10, %rcx     # %rcx <- %rax - %r10
    11      addq %r10, %rax     # %rax <- %rax + %r10
    12      andq %r10, %r10     # Compare %r10 with 0
    13      cmovl %rcx, %rax    # If %r10 is less than 0, %rax <- %rcx
    14      addq %r8, %rdi      # start++
    15      subq %r9, %rsi      # count--. Set CC
    16  test:
    17      jne loop            # Stop when 0
    18      ret                 # Return
    ```

- Modified answer:
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
    11      cmovg %r11, %r10
    12      addq %r10, %rax     # %rax <- %rax + %r10
    13      addq %r8, %rdi      # start++
    14      subq %r9, %rsi      # count--. Set CC
    15  test:
    16      jne loop            # Stop when 0
    17      ret                 # Return
    ```
