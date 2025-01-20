# Practice Problem 4.30 (solution page 490)
Write HCL code for the signal `f_stat`, providing the provisional status for the fetched instruction.

## Solution:
```
word f_stat = [
    imem_error          : SADR;
    #!f_instr_valid     : SINS;
    !instr_valid        : SINS;
    f_icode == IHALT    : SHLT 
    1                   : SAOK;
];
```