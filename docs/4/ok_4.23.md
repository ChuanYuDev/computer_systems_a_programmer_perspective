# Practice Problem 4.23 (solution page 488)
Based on the first operand of the first step of the execute stage in Figures 4.18 to 4.21, write an HCL description for the signal `aluB` in SEQ.

## Solution:
```
word aluB = [
    icode in { IOPQ, IRMMOVQ, IMRMOVQ, IPUSHQ, IPOPQ, ICALL, IRET } : valB;
    icode in { IRRMOVQ, IIRMOVQ }                                   : 0;
    # Other instructions don't need ALU
];
```