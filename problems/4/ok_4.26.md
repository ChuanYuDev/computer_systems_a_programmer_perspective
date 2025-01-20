# Practice Problem 4.26 (solution page 489)
We want to set the control signal `mem_write` only for instructions that write data to memory. Write HCL code for the signal `mem_write` in SEQ.

## Solution:
```
bool mem_write = icode in { IRMMOVQ, IPUSHQ, ICALL };
```