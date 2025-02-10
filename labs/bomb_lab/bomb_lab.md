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
- Generate the symbol table and store it in the file `symbol_table`

    ```
    objdump -t bomb > symbol_table
    ```

- Disassemble all of the code in the bomb and store in `disassemble`

    ```
    objdump -d bomb > disassemble
    ```

- Display the printable strings in the bomb and store in `strings`

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
    => 0x0000000000400ee0 <+0>:     sub    $0x8,%rsp
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
### Phase 3
### Phase 4
### Phase 5
### Phase 6