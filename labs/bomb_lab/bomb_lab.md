# Bomb Lab
## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [Solution](#solution)
    - [Phase 1](#phase-1)
    - [Phase 2](#phase-2)
    - [Phase 3](#phase-3)
    - [Phase 4](#phase-4)
    - [Phase 5](#phase-5)
    - [Phase 6](#phase-6)
    - [Secret phase](#secret-phase)

## Introduction
- A "binary bomb" is a program provided to students as an object code file
    - When run, it prompts the user to type in 6 different strings
    - If any of these is incorrect, the bomb "explodes," printing an error message and logging the event on a grading server
    - Students must "defuse" their own unique bomb by disassembling and reverse engineering the program to determine what the 6 strings should be

- The lab teaches students to understand assembly language, and also forces them to learn how to use a debugger
    - It's also great fun
    - A legendary lab among the CMU undergrads

- Here's a Linux/x86-64 binary bomb that you can try out for yourself
    - The feature that notifies the grading server has been disabled, so feel free to explode this bomb with impunity
    - If you're an instructor with a CS:APP account, then you can download the solution

## Duration
- 2025/2/4 - 

## Solution
- Generate the symbol table and store it in the file `bomb/symbol_table`

    ```
    objdump -t bomb > symbol_table
    ```

- Disassemble all of the code in the bomb and store in `bomb/disassemble`

    ```
    objdump -d bomb > disassemble
    ```

- Display the printable strings in the bomb and store in `bomb/strings`

    ```
    strings bomb > strings
    ```

### Phase 1
- Debug with `gdb`

    ```
    linux> gdb bomb

    (gdb) break phase_1
    Breakpoint 1 at 0x400ee0
    (gdb) run
    Welcome to my fiendish little bomb. You have 6 phases with
    which to blow yourself up. Have a nice day!
    sdf

    Breakpoint 1, 0x0000000000400ee0 in phase_1 ()
    (gdb) disas phase_1
    Dump of assembler code for function phase_1:
    =>  0x0000000000400ee0 <+0>:     sub    $0x8,%rsp
        0x0000000000400ee4 <+4>:     mov    $0x402400,%esi
        0x0000000000400ee9 <+9>:     call   0x401338 <strings_not_equal>
        0x0000000000400eee <+14>:    test   %eax,%eax
        0x0000000000400ef0 <+16>:    je     0x400ef7 <phase_1+23>
        0x0000000000400ef2 <+18>:    call   0x40143a <explode_bomb>
        0x0000000000400ef7 <+23>:    add    $0x8,%rsp
        0x0000000000400efb <+27>:    ret    
    End of assembler dump.

    (gdb) stepi
    0x0000000000400ee4 in phase_1 ()
    (gdb) stepi
    0x0000000000400ee9 in phase_1 ()

    (gdb) print /x $rdi
    $3 = 0x603780
    (gdb) print /x $rsi
    $4 = 0x402400
    ```

- From disassembling `phase_1`, we get that it calls `strings_not_equal` function
    - The first argument `input` is the same as `phase_1` stored at `%rdi` with the value `0x603780`

    - The second argument is assigned at `0x400ee4` instruction `mov    $0x402400,%esi` stored at `%rsi` with the value `0x402400`

    - The `strings_not_equal` function will check whether or not the two strings stored at the address `0x603780` (input) and `0x402400` respectively are equal

- Examine the string stored at `0x402400`

    ```
    (gdb) x/s 0x402400
    0x402400:       "Border relations with Canada have never been better."
    ```

- Therefore, the phase 1 string is 

    ```
    Border relations with Canada have never been better.
    ```

### Phase 2
- `phase_2` machine code from `disassemble` file

    ![](./images/phase_2.png)
    - Line 361 call function `read_six_numbers`
    - First argument `%rdi` is `input`
    - Second argument `%rsi` is `%rsp`

- Function `read_six_numbers`

    - The machine code from `disassemble` file

        ![](./images/read_six_numbers.png)

    - The stack frame diagram

        ![](./images/read_six_numbers_stack.png)
    
    - `read_six_numbers` calls function `sscanf`
        - Argument 1 `rdi`: `input`
        - Argument 2 `rsi`: `0x4025c3`

            ```
            (gdb) x/s 0x4025c3
            0x4025c3:       "%d %d %d %d %d %d"
            ```
        
        - Argument 3 `rdx`: `-0x28`
            - This is the relative address based on the `phase_2` stack frame largest address, arguments 4-8 are similar

        - Argument 4 `rcx`: `-0x24`
        - Argument 5 `r8`: `-0x20`
        - Argument 6 `r9`: `-0x1c`
        - Argument 7 `(%rsp)`: `-0x18`
        - Argument 8 `0x8(%rsp)`: `-0x14`
    
    - The `read_six_numbers` will read 6 integers of `int` type to fill the caller stack frame from `-0x28` to `-0x14`, `0x4` size each

- `phase_2` stack frame diagram after Line 361

    ![](./images/phase_2_stack.png)
    - Based on the machine code, we can get the values of 6 integers from "relative" address `-0x28` to `-0x14` must be `1`, `2`, `4`, `8`, `16`, and `32`, otherwise the bomb explodes

- Therefore, the phase 2 string is 

    ```
    1 2 4 8 16 32
    ```

### Phase 3
- Function `phase_3`
    - The machine code from `disassemble` file

        ![](./images/phase_3.png)

    - The stack frame diagram

        ![](./images/phase_3_stack.png)

    - `phase_3` will first call function `sscanf`

        - Argument 1 `rdi`: `input`
        - Argument 2 `rsi`: `0x4025cf`

            ```
            (gdb) x/s 0x4025cf
            0x4025cf:       "%d %d"
            ```
        
        - Argument 3 `rdx`: `-0x10`
        - Argument 4 `rcx`: `-0xc`

    - `sscanf` will read two integers from `input` string and stored them in the stack frame `-0x10` and `-0xc`

    - Line 394, `ja` is intended for comparing two integers of `unsigned` type, therefore the integer stored at `-0x10` should be in the range of `[0, 0x7]`

    - Line 396, the memory values starting from `0x402470` are eight addresses in `phase_3`

        ```
        (gdb) x/8g 0x402470
        0x402470:       0x0000000000400f7c      0x0000000000400fb9
        0x402480:       0x0000000000400f83      0x0000000000400f8a
        0x402490:       0x0000000000400f91      0x0000000000400f98
        0x4024a0:       0x0000000000400f9f      0x0000000000400fa6
        ```
    
    - The integer stored at `-0x10` can be any value between `[0, 0x7]`, the `jmp` instruction will jump to corresponding address, and assigned one value to `%eax`

    - The value at address `-0xc` must be equal to the `%eax`

- Therefore one of the `phase_3` string is 

    ```
    0 207
    ```

### Phase 4
### Phase 5
### Phase 6
### Secret phase