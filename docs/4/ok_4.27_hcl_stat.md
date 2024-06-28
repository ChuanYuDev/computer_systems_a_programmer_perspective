# Practice Problem 4.27 (solution page 489)
Write HCL code for `Stat`, generating the four status codes `SAOK`, `SADR`, `SINS`, and `SHLT` (see Figure 4.26).

## Solution:

```
word Stat = [
    imem_error || dmem_error    : SADR;
    !instr_valid                : SINS;
    # icode == 0                : SHLT 
    icode == IHALT              : SHLT 
    1                           : SAOK;
];
```
- Is `Stat` type `word`? `word`: 64-bit, only 4-bit is enough