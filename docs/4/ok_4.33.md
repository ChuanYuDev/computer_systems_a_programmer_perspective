# Practice Problem 4.33 (solution page 491)
Suppose the order of the fifth and sixth cases (the two forwarding sources from the write-back stage) in the HCL code for `d_valA` were reversed. Write a Y86-64 program that would be executed incorrectly. Describe how the error would occur and its effect on the program behavior.

## Solution:

```
irmovq $5 , %rdx
irmovq $0x100 , %rsp
rmmovq %rdx , 0 (%rsp)
popq %rsp
nop
nop
rrmovq %rsp , %rax
```

- When `popq %rsp` is in the write-back stage, `rrmovq` is in the decode stage
- `rrmovq` will fetch `%rsp` via forwarding which is `W_valE` (value: ~~`0x98`~~ `0x108`) not `W_valM` (value: `5`) if the order were reversed
- This will cause `rrmovq` to set `rax` to ~~`0x98`~~ `0x108` not `5`