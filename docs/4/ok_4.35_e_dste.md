# Practice Problem 4.35 (solution page 491)
Our second case in the HCL code for `d_valA` uses signal `e_dstE` to see whether to select the ALU output `e_valE` as the forwarding source. Suppose instead that we use signal `E_dstE`, the destination register ID in pipeline register E for this selec6on. Write a Y86-64 program that would give an incorrect result with this modified forwarding logic.

## Solution:

- ~~Assue condition codes are `100`~~
- Y86-64 program:
    ```
    irmovq $5, %rax
    irmovq $10, %rdx
    xorq %rcx, %rcx     # CC = 100
    cmovne %rdx, %rax
    # rrmovq %rax, %rcx
    addq %rax, %rax
    ```
- HCL for `e_dstE`
    ```
    word e_dstE = [
        E_icode in { IRRMOVQ } && e_Cnd : E_dstE;
        1                               : RNONE; # Don't write any register
    ];
    ```

- When `cmovne` in the execute stage, `E_dstE` is `%rax`, `e_dstE` is RNONE because the condition is not satisfied

- ~~If we use `E_dstE` instead of `e_dstE`, `rrmovq` will set `%rax` to `10` not `5` which will cause `%rcx` to `10` not `5`~~

- If we use `E_dstE` instead of `e_dstE` for `d_valA`, `d_valA` will be set to `10` even the condition is not satisfied, `d_valB` will corretly be set to `5`
    - The program will set `rax` to `15` not `10`