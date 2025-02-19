# Attack Lab
## Table of contents
- [Introduction](#introduction)
- [Duration](#duration)
- [`ctarget` segmentation fault bug on Ubuntu 22.04](#ctarget-segmentation-fault-bug-on-ubuntu-2204)

- [Solution](#solution)
    - [Phase 1](#phase-1)
    - [Phase 2](#phase-2)
    - [Phase 3](#phase-3)
    - [Phase 4](#phase-4)
    - [Phase 5](#phase-5)

## Introduction
- Note: This is the 64-bit successor to the 32-bit Buffer Lab

- Students are given a pair of unique custom-generated x86-64 binary executables, called **targets**, that have buffer overflow bugs
    - One target is vulnerable to code injection attacks
    
    - The other is vulnerable to return-oriented programming attacks
    
    - Students are asked to modify the behavior of the targets by developing exploits based on either code injection or return-oriented programming
    
- This lab teaches the students about the stack discipline and teaches them about the danger of writing code that is vulnerable to buffer overflow attacks

- If you're a self-study student, here are a pair of Ubuntu 12.4 targets that you can try out for yourself
    - You'll need to run your targets using the `-q` option so that they don't try to contact a non-existent grading server
    
    - If you're an instructor with a CS:APP acount, you can download the solutions here

## Duration
- 2025/2/19 - Due: 2025/2/28

## `ctarget` segmentation fault bug on Ubuntu 22.04
- No matter what strings you input or whether or not you input string, the `ctarget` will always cause segmentation fault

    ```
    (gdb) run -q
    Starting program: /home/yuchuan/projects/computer_systems_a_programmer_perspective/labs/attack_lab/target1/ctarget -q
    [Thread debugging using libthread_db enabled]
    Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
    Cookie: 0x59b997fa

    Program received signal SIGSEGV, Segmentation fault.
    0x00007ffff7dff0d0 in __vfprintf_internal (s=0x7ffff7fa5780 <_IO_2_1_stdout_>, format=0x4032b4 "Type string:", ap=ap@entry=0x5561dbd8, mode_flags=mode_flags@entry=2) at ./stdio-common/vfprintf-internal.c:1244
    1244    ./stdio-common/vfprintf-internal.c: No such file or directory.

    (gdb) backtrace
    #0  0x00007ffff7dff0d0 in __vfprintf_internal (s=0x7ffff7fa5780 <_IO_2_1_stdout_>, format=0x4032b4 "Type string:", ap=ap@entry=0x5561dbd8, 
        mode_flags=mode_flags@entry=2) at ./stdio-common/vfprintf-internal.c:1244
    #1  0x00007ffff7ebec4b in ___printf_chk (flag=flag@entry=1, format=format@entry=0x4032b4 "Type string:") at ./debug/printf_chk.c:33
    #2  0x0000000000401f10 in printf (__fmt=0x4032b4 "Type string:") at /usr/include/x86_64-linux-gnu/bits/stdio2.h:105
    #3  launch (offset=<optimized out>) at support.c:293
    #4  0x0000000000401ffa in stable_launch (offset=<optimized out>) at support.c:340
    Backtrace stopped: Cannot access memory at address 0x55686000
    ```

- Based on [Rijuyuezhu's blog](https://blog.rijuyuezhu.top/posts/db646f34/), it is due to an instruction in `__vfprintf_internal` of `glibc`, namely `movaps %xmm1, 0x10(%rsp)`
    - This instruction needs its memory operand `0x10(%rsp)` corresponding an address divisible by 16
    
    - I guess that due to the compiling of `ctarget` is on a old convention, where the size of function frame is not strictly required to be a multiple of 16
    
    - I tried to manually modify the low bits of `%rsp` through `gdb`, and it eliminates the `SIGSEGV`, confirming my guessing

- Get self-made `printf.so`
    - First of all, we can find a naive `printf` implementation (For me, I use the programming homework in NJU PA)
    
    - Then we can compile it to be a shared library(suppose the source file is named `printf.c`):

        ```
        gcc -shared -fPIC -o printf.so printf.c
        ```

    - Notice that we shall provide interface `__printf_chk` and `__sprintf_chk`

- Use the `LD_PRELOAD` to substitute the default `glibc` shared library
    - Put `printf.so` in the same directory of `ctarget`, then use command

        ```
        LD_PRELOAD=./printf.so ./ctarget -q
        ```

    - And it works! 

- **What is the relation between the self-made `printf.so` and `movaps` instruction?**
    - Does self-made `printf.so` ensure the stack frame address satisfy a 16-byte alignment?

- How to use `gdb` with `LD_PRELOAD`

    ```
    gdb your_program

    (gdb) set environment LD_PRELOAD ./printf.so
    (gdb) start
    ```

## Solution
- Disassemble `ctarget` and store it in the file `ctarget_disassemble`

    ```
    objdump -d ctarget > ctarget_disassemble
    ```
### Phase 1
- `getbuf`
    - The machine code

        ```
        00000000004017a8 <getbuf>:
        4017a8:	48 83 ec 28          	sub    $0x28,%rsp
        4017ac:	48 89 e7             	mov    %rsp,%rdi
        4017af:	e8 8c 02 00 00       	call   401a40 <Gets>
        4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
        4017b9:	48 83 c4 28          	add    $0x28,%rsp
        4017bd:	c3                   	ret    
        4017be:	90                   	nop
        4017bf:	90                   	nop
        ```
    
    - The stack frame

        ![](./images/getbuf_stack_frame.png)

- `touch1`
    - The machine code

        ```
        00000000004017c0 <touch1>:
        4017c0:	48 83 ec 08          	sub    $0x8,%rsp
        4017c4:	c7 05 0e 2d 20 00 01 	movl   $0x1,0x202d0e(%rip)        # 6044dc <vlevel>
        4017cb:	00 00 00 
        4017ce:	bf c5 30 40 00       	mov    $0x4030c5,%edi
        4017d3:	e8 e8 f4 ff ff       	call   400cc0 <puts@plt>
        4017d8:	bf 01 00 00 00       	mov    $0x1,%edi
        4017dd:	e8 ab 04 00 00       	call   401c8d <validate>
        4017e2:	bf 00 00 00 00       	mov    $0x0,%edi
        4017e7:	e8 54 f6 ff ff       	call   400e40 <exit@plt>
        ```

- We need to fill 0x28 (40) bytes of `getbuf` stack frame and then overwrite the return address to `touch1` first instruction address
    - [Exploit string](./target1/phase_1_exploit.txt)

    - Generate exploit raw string

        ```
        ./hex2raw < phase_1_exploit.txt > phase_1_exploit_rax.txt
        ```

    - Apply the raw string to `ctarget`

        ```
        LD_PRELOAD=./printf.so ./ctarget -q -i phase_1_exploit_rax.txt
        ```

### Phase 2
### Phase 3
### Phase 4
### Phase 5